#include "World3D.hpp"
#include "scene_lua.hpp"
using namespace std;

#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"
#include "GeometryNode.hpp"
#include "JointNode.hpp"
#include "general.hpp"

#include <imgui/imgui.h>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

static bool show_gui = true;

static const float gravity = -9.8f;
static const float transAm = 0.05f;

const size_t CIRCLE_PTS = 48;

//----------------------------------------------------------------------------------------
// Constructor
World3D::World3D(const std::string & luaSceneFile)
	: m_luaSceneFile(luaSceneFile),
	  m_positionAttribLocation(0),
	  m_normalAttribLocation(0),
	  m_vao_meshData(0),
	  m_vbo_vertexPositions(0),
	  m_vbo_vertexNormals(0),
	  m_vao_arcCircle(0),
	  m_vbo_arcCircle(0),
	  m_celshading(true),
	  m_texturemapping(true),
	  m_bumpmapping(true),
	  m_mousePressPos(vec3(0,0,0)),
	  m_projectedMousePressPos(vec3(0,0,0)),
	  m_currentviewAngle(0.0f),
	  m_currentviewDistance(vec3(0,0,0)),
	  m_distanceToPuppet(vec3(0,0,0))
{
	m_pressed[LEFT] = NONE;
	m_pressed[MIDDLE] = NONE;
	m_pressed[RIGHT] = NONE;

	m_keypressed[LEFT_ARROW] = NO_KEY;
	m_keypressed[UP_ARROW] = NO_KEY;
	m_keypressed[DOWN_ARROW] = NO_KEY;
	m_keypressed[RIGHT_ARROW] = NO_KEY;
	m_keypressed[CTRL_KEY] = NO_KEY;
	
	time(&m_beginTime);

	string file = "Textures/grass.jpg";
	grass = new Texture(file);

	file = "Textures/stonehouse.jpg";
	stonehouse = new Texture(file);

	file = "Textures/wood.jpg";
	wood = new Texture(file);

	file = "Textures/soil.jpg";
	soil = new Texture(file);

	file = "Textures/metal.jpg";
	metal = new Texture(file);

	file = "Textures/treeneedles.jpg";
	treeneedles = new Texture(file);
	
	file = "Textures/gravel.jpg";
	gravel = new Texture(file);

	file = "Textures/gravel_normalmap.png";
	gravel_normalmap = new Texture(file);
};

//----------------------------------------------------------------------------------------
// Destructor
World3D::~World3D()
{
	delete m_char;
	for(DynamicObject * object: m_dobjects)
	{
		delete object;
	}
	delete grass;
	delete stonehouse;
	delete wood;
	delete soil;
	delete metal;
	delete treeneedles;
	delete gravel;
	delete gravel_normalmap;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void World3D::init()
{
	// Set the background colour.
	glClearColor(0.3, 0.5, 0.7, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao_arcCircle);
	glGenVertexArrays(1, &m_vao_meshData);
	enableVertexShaderInputSlots();

	processLuaSceneFile(m_luaSceneFile);

	grass->load();
	stonehouse->load();
	wood->load();
	soil->load();
	metal->load();
	treeneedles->load();
	gravel->load();
	gravel_normalmap->load();

	// Load and decode all .obj files at once here.  You may add additional .obj files to
	// this list in order to support rendering additional mesh types.  All vertex
	// positions, and normals will be extracted and stored within the MeshConsolidator
	// class.
	unique_ptr<MeshConsolidator> meshConsolidator (new MeshConsolidator{
			getAssetFilePath("cube.obj"),
			getAssetFilePath("cylinder.obj"),
			getAssetFilePath("sphere.obj"),
			getAssetFilePath("suzanne.obj"),
			getAssetFilePath("tree.obj"),
			getAssetFilePath("corn.obj")
	});

	// Acquire the BatchInfoMap from the MeshConsolidator.
	meshConsolidator->getBatchInfoMap(m_batchInfoMap);

	// Take all vertex data within the MeshConsolidator and upload it to VBOs on the GPU.
	uploadVertexDataToVbos(*meshConsolidator);

	mapVboDataToVertexShaderInputLocations();

	initPerspectiveMatrix();
	initViewMatrix();
	initLightSources();

	// Exiting the current scope calls delete automatically on meshConsolidator freeing
	// all vertex data resources.  This is fine since we already copied this data to
	// VBOs on the GPU.  We have no use for storing vertex data on the CPU side beyond
	// this point.
}

//----------------------------------------------------------------------------------------
void World3D::processLuaSceneFile(const std::string & filename) {
	// This version of the code treats the Lua file as an Asset,
	// so that you'd launch the program with just the filename
	// of a puppet in the Assets/ directory.
	// std::string assetFilePath = getAssetFilePath(filename.c_str());
	// m_rootNode = std::shared_ptr<SceneNode>(import_lua(assetFilePath));

	// This version of the code treats the main program argument
	// as a straightforward pathname.
	m_rootNode = std::shared_ptr<SceneNode>(import_lua(filename));
	if (!m_rootNode) {
		std::cerr << "Could not open " << filename << std::endl;
	}
	
	for(SceneNode * child : m_rootNode->children) {
		if(child->m_name == "puppet")
			m_puppet = child;
		else if(child->m_name == "world")
			m_world = child;
	}

	m_movingDoor = getNode(m_world, "movingDoor");
	DynamicObject *dobject = new DynamicObject(m_movingDoor, Axis::Z, m_movingDoor->distanceFromOrigin, vec3(0,0,2.2), vec3(0,0,0.005));
	m_dobjects.push_back(dobject);

	m_distanceToPuppet = m_puppet->distanceFromOrigin; // need to store distance from view (ie. camera) to puppet for rotation purposes
	
	m_char = new Character(m_puppet);
}

//----------------------------------------------------------------------------------------
void World3D::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();

	m_shader_arcCircle.generateProgramObject();
	m_shader_arcCircle.attachVertexShader( getAssetFilePath("arc_VertexShader.vs").c_str() );
	m_shader_arcCircle.attachFragmentShader( getAssetFilePath("arc_FragmentShader.fs").c_str() );
	m_shader_arcCircle.link();
}

//----------------------------------------------------------------------------------------
void World3D::enableVertexShaderInputSlots()
{
	//-- Enable input slots for m_vao_meshData:
	{
		glBindVertexArray(m_vao_meshData);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_positionAttribLocation = m_shader.getAttribLocation("position");
		glEnableVertexAttribArray(m_positionAttribLocation);

		// Enable the vertex shader attribute location for "normal" when rendering.
		m_normalAttribLocation = m_shader.getAttribLocation("normal");
		glEnableVertexAttribArray(m_normalAttribLocation);

		CHECK_GL_ERRORS;
	}


	//-- Enable input slots for m_vao_arcCircle:
	{
		glBindVertexArray(m_vao_arcCircle);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_arc_positionAttribLocation = m_shader_arcCircle.getAttribLocation("position");
		glEnableVertexAttribArray(m_arc_positionAttribLocation);

		CHECK_GL_ERRORS;
	}

	// Restore defaults
	glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------
void World3D::uploadVertexDataToVbos (
		const MeshConsolidator & meshConsolidator
) {
	// Generate VBO to store all vertex position data
	{
		glGenBuffers(1, &m_vbo_vertexPositions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexPositionBytes(),
				meshConsolidator.getVertexPositionDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store all vertex normal data
	{
		glGenBuffers(1, &m_vbo_vertexNormals);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexNormalBytes(),
				meshConsolidator.getVertexNormalDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store the trackball circle.
	{
		glGenBuffers( 1, &m_vbo_arcCircle );
		glBindBuffer( GL_ARRAY_BUFFER, m_vbo_arcCircle );

		float *pts = new float[ 2 * CIRCLE_PTS ];
		for( size_t idx = 0; idx < CIRCLE_PTS; ++idx ) {
			float ang = 2.0 * M_PI * float(idx) / CIRCLE_PTS;
			pts[2*idx] = cos( ang );
			pts[2*idx+1] = sin( ang );
		}

		glBufferData(GL_ARRAY_BUFFER, 2*CIRCLE_PTS*sizeof(float), pts, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void World3D::mapVboDataToVertexShaderInputLocations()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_meshData);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexPositions" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);
	glVertexAttribPointer(m_positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexNormals" into the
	// "normal" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);
	glVertexAttribPointer(m_normalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;

	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_arcCircle);

	// Tell GL how to map data from the vertex buffer "m_vbo_arcCircle" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_arcCircle);
	glVertexAttribPointer(m_arc_positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void World3D::initPerspectiveMatrix()
{
	float aspect = ((float)m_windowWidth) / m_windowHeight;
	m_perpsective = glm::perspective(degreesToRadians(60.0f), aspect, 0.1f, 30.0f);
}


//----------------------------------------------------------------------------------------
void World3D::initViewMatrix() {
	m_view = glm::lookAt(vec3(0.0f, 0.0f, 0.5f), vec3(0.0f, 0.0f, -1.0f),
			vec3(0.0f, 1.0f, 0.0f));

	m_model = m_view;
}

//----------------------------------------------------------------------------------------
void World3D::initLightSources() {
	// World-space position
	m_light.position = vec3(0.2f, 0.5f, 0.5f);
	m_light.rgbIntensity = vec3(0.8f); // White light
}

void World3D::initShadowData()
{

}

//----------------------------------------------------------------------------------------
void World3D::uploadCommonSceneUniforms() 
{
	m_shader.enable();
	{
		//-- Set Perpsective matrix uniform for the scene:
		GLint location = m_shader.getUniformLocation("Perspective");
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_perpsective));
		CHECK_GL_ERRORS;


		//-- Set LightSource uniform for the scene:
		{
			location = m_shader.getUniformLocation("light.position");
			glUniform3fv(location, 1, value_ptr(m_light.position));
			location = m_shader.getUniformLocation("light.rgbIntensity");
			glUniform3fv(location, 1, value_ptr(m_light.rgbIntensity));
			CHECK_GL_ERRORS;
		}

		//-- Set background light ambient intensity
		{
			location = m_shader.getUniformLocation("ambientIntensity");
			vec3 ambientIntensity(0.1f);
			glUniform3fv(location, 1, value_ptr(ambientIntensity));
			CHECK_GL_ERRORS;
		}
	}
	m_shader.disable();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void World3D::appLogic()
{

	moveCharacter();

	for(DynamicObject *object : m_dobjects)
	{
		vec3 threshold = object->m_object->distanceFromOrigin - m_puppet->distanceFromOrigin;

		object->move();

		if(length(threshold) < 4 && collided(m_puppet, object->m_object))
		{
			object->m_movementAmount = vec3(-object->m_movementAmount.x,-object->m_movementAmount.y,-object->m_movementAmount.z);
		}
		else if(object->m_object->m_name == "movingDoor" && 
			object->m_movementAmount[object->m_axis] < 0.0f && 
			(length(object->m_object->distanceFromOrigin-object->m_initPosn) < 0.05f || 
			 length(object->m_object->distanceFromOrigin-object->m_initPosn) > object->m_max[object->m_axis]-0.01f) &&
			length(object->m_object->distanceFromOrigin-m_puppet->distanceFromOrigin) < 4)
		{
			system("canberra-gtk-play -f close_door_2.wav &");
		}
	}

	initLightSources();
	uploadCommonSceneUniforms();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void World3D::guiLogic()
{
	if( !show_gui ) {
		return;
	}

	static bool firstRun(true);
	if (firstRun) {
		ImGui::SetNextWindowPos(ImVec2(50, 50));
		firstRun = false;
	}

	static bool showDebugWindow(true);
	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);
	float opacity(0.5f);

	ImGui::Begin("Properties", &showDebugWindow, ImVec2(100,200), opacity,
		windowFlags);

		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		string message;
		if(m_celshading)
		{
			message = "Cel Shading Enabled (C)";
			ImGui::TextColored( ImColor(0, 255, 0), "%s", message.c_str() );
		}
		else
		{
			message = "Cel Shading Disabled (C)";
			ImGui::TextColored( ImColor(255, 0, 0), "%s", message.c_str() );
		}
		
		if(m_texturemapping)
		{
			message = "Texture Mapping Enabled (T)";
			ImGui::TextColored( ImColor(0, 255, 0), "%s", message.c_str() );
		}
		else
		{
			message = "Texture Mapping Disabled (T)";
			ImGui::TextColored( ImColor(255, 0, 0), "%s", message.c_str() );
		}
		
		if(m_bumpmapping)
		{
			message = "Bump Mapping Enabled (B)";
			ImGui::TextColored( ImColor(0, 255, 0), "%s", message.c_str() );
		}
		else
		{
			message = "Bump Mapping Disabled (B)";
			ImGui::TextColored( ImColor(255, 0, 0), "%s", message.c_str() );
		}
		
		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );
		
		time_t end;
		time(&end);
		
		ImGui::Text( "Game Time: %.1f seconds", difftime(end,m_beginTime) );

	ImGui::End();
}

//----------------------------------------------------------------------------------------
// Update mesh specific shader uniforms:
void World3D::updateShaderUniforms(
		const ShaderProgram & shader,
		const GeometryNode & node,
		const glm::mat4 & viewMatrix
) {

	shader.enable();
	{
		//-- Set ModelView matrix:
		GLint location = shader.getUniformLocation("ModelView");
		mat4 modelView = viewMatrix * node.trans;
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelView));
		CHECK_GL_ERRORS;

		//-- Set Model matrix:
		location = shader.getUniformLocation("Model");
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_model));
		CHECK_GL_ERRORS;

		//-- Set NormMatrix:
		location = shader.getUniformLocation("NormalMatrix");
		mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelView)));
		glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(normalMatrix));
		CHECK_GL_ERRORS;

		location = shader.getUniformLocation("use_celshading");
		glUniform1i(location,m_celshading);

		GLint use = 0;
		location = shader.getUniformLocation("use_texture");
		if(m_texturemapping)
		{
			if(node.m_name == "wall" 
				|| node.m_name == "outerwall" 
				|| node.m_name == "building"
				|| node.m_name == "foundation"
				|| node.m_name == "pillar"
				/*|| node.m_name == "bumpcube"*/) // objects that should have stone texture
			{
				glBindTexture(GL_TEXTURE_2D, stonehouse->m_textureId);
				use = 1;
			}
			else if(node.m_name == "roof")
			{
				glBindTexture(GL_TEXTURE_2D, wood->m_textureId);
				use = 1;
			}
			else if(node.m_name == "soil")
			{
				glBindTexture(GL_TEXTURE_2D, soil->m_textureId);
				use = 1;
			}
			else if(node.m_name == "movingDoor")
			{
				glBindTexture(GL_TEXTURE_2D, metal->m_textureId);
				use = 1;
			}
			else if(node.m_name == "tree")
			{
				glBindTexture(GL_TEXTURE_2D, treeneedles->m_textureId);
				use = 1;
			}
			else if(node.m_name == "ground")
			{
				glBindTexture(GL_TEXTURE_2D, grass->m_textureId);
				use = 1;
			}
			else if(node.m_name == "pavement" || node.m_name == "bumpcube")
			{
				glBindTexture(GL_TEXTURE_2D, gravel->m_textureId);
				use = 1;
			}
		}
		glUniform1i(location,use);
		CHECK_GL_ERRORS;

		GLint use_bump = 0;
		location = shader.getUniformLocation("use_bump");
		if(m_bumpmapping)
		{
			if(node.m_name == "bumpcube")
			{
				glBindTexture(GL_TEXTURE_2D, gravel_normalmap->m_textureId);
				use_bump = 1;
			}
		}
		glUniform1i(location,use_bump);
		CHECK_GL_ERRORS;

		//-- Set Material values:
		location = shader.getUniformLocation("material.kd");
		vec3 kd = node.material.kd;
		glUniform3fv(location, 1, value_ptr(kd));
		CHECK_GL_ERRORS;
		location = shader.getUniformLocation("material.ks");
		vec3 ks = node.material.ks;
		glUniform3fv(location, 1, value_ptr(ks));
		CHECK_GL_ERRORS;
		location = shader.getUniformLocation("material.shininess");
		glUniform1f(location, node.material.shininess);
		CHECK_GL_ERRORS;

	}
	shader.disable();

}

void World3D::characterJump()
{
	if(m_keypressed[DOWN_ARROW] == DOWN_ARROW || m_keypressed[UP_ARROW] == NO_KEY) return;

	m_char->m_midair = true;
	m_char->m_theta = 30.0f;

	m_char->m_initPosn = m_puppet->distanceFromOrigin;
	m_char->m_yvelocity = 100*transAm*sin(degreesToRadians(m_char->m_theta));
	m_char->m_xstart = 0.0f;
	m_char->m_metric = 1;
}

bool World3D::checkCollision()
{
	for(SceneNode * child : m_world->children)
	{
		vec3 threshold = child->distanceFromOrigin - m_puppet->distanceFromOrigin;
		
		// todo -> make the efficiency checking better
		if(child->m_name == "ground" || (length(threshold) > 6 && child->m_name != "outerwall") || (length(threshold) > 1 && child->m_name == "tree")) 
			continue;

		if(collided(m_puppet, child))
		{
			return true;
		}
	}

	return false;
}

void World3D::moveCamera()
{
	float rotAm = 1.0f;

	if(m_keypressed[UP_ARROW] == UP_ARROW)
	{
		m_view = glm::translate(m_view, -m_currentviewDistance+m_distanceToPuppet);
		m_view = glm::rotate(m_view, m_currentviewAngle, vec3(0.0,1.0f,0));
		m_view = glm::rotate(m_view, degreesToRadians(rotAm), vec3(1.0,0.0f,0));
		m_view = glm::rotate(m_view, -m_currentviewAngle, vec3(0.0,1.0f,0));
		m_view = glm::translate(m_view, m_currentviewDistance-m_distanceToPuppet);
	}
	if(m_keypressed[LEFT_ARROW] == LEFT_ARROW)
	{
		m_view = glm::translate(m_view, -m_currentviewDistance+m_distanceToPuppet);
		m_view = glm::rotate(m_view, degreesToRadians(-rotAm), vec3(0,1.0f,0));
		m_view = glm::translate(m_view, m_currentviewDistance-m_distanceToPuppet);
	}
	if(m_keypressed[RIGHT_ARROW] == RIGHT_ARROW)
	{
		m_view = glm::translate(m_view, -m_currentviewDistance+m_distanceToPuppet);
		m_view = glm::rotate(m_view, degreesToRadians(rotAm), vec3(0,1.0f,0));
		m_view = glm::translate(m_view, m_currentviewDistance-m_distanceToPuppet);
	}
	if(m_keypressed[DOWN_ARROW] == DOWN_ARROW)
	{
		m_view = glm::translate(m_view, -m_currentviewDistance+m_distanceToPuppet);
		m_view = glm::rotate(m_view, m_currentviewAngle, vec3(0.0,1.0f,0));
		m_view = glm::rotate(m_view, degreesToRadians(-rotAm), vec3(1.0,0.0f,0));
		m_view = glm::rotate(m_view, -m_currentviewAngle, vec3(0.0,1.0f,0));
		m_view = glm::translate(m_view, m_currentviewDistance-m_distanceToPuppet);
	}
}

void World3D::moveCharacter()
{
	float rotAm = 1.0f;
	float transAmBkw = transAm/3;
	bool collision;

	if(m_keypressed[CTRL_KEY] == CTRL_KEY)
	{
		moveCamera();
		if(!m_char->m_midair) return;
	}
	
	if(m_char->m_midair) 
	{
		calculateTrajectoryHeight();
	}

	if(m_keypressed[UP_ARROW] == UP_ARROW || m_char->m_midair)
	{
		m_char->m_xstart += (m_char->m_metric)*transAm;
		m_currentviewDistance += vec3(transAm*sin(degreesToRadians(m_currentviewAngle)),0,transAm*cos(degreesToRadians(m_currentviewAngle)));
		m_view = glm::translate(m_view, vec3(transAm*sin(degreesToRadians(m_currentviewAngle)),0,transAm*cos(degreesToRadians(m_currentviewAngle))));
		m_puppet->translate(vec3(-transAm*sin(degreesToRadians(m_currentviewAngle)),0,-transAm*cos(degreesToRadians(m_currentviewAngle))));

		collision = checkCollision();
		if(collision)
		{
			if(m_char->m_midair)
			{
				// if we hit something we go directly to the ground, no questions asked
				float distanceFromGround = m_puppet->distanceFromOrigin.y;
				m_puppet->translate(vec3(0,-distanceFromGround+m_char->startingHeight,0));
				m_char->refreshJump(m_currentviewAngle);
			}
			float transAmNeg = -transAm;
			m_currentviewDistance += vec3(transAmNeg*sin(degreesToRadians(m_currentviewAngle)),0,transAmNeg*cos(degreesToRadians(m_currentviewAngle)));
			m_view = glm::translate(m_view, vec3(transAmNeg*sin(degreesToRadians(m_currentviewAngle)),0,transAmNeg*cos(degreesToRadians(m_currentviewAngle))));
			m_puppet->translate(vec3(-transAmNeg*sin(degreesToRadians(m_currentviewAngle)),0,-transAmNeg*cos(degreesToRadians(m_currentviewAngle))));
		}

		m_char->runAnimation(m_currentviewAngle);
	}
	if(m_keypressed[LEFT_ARROW] == LEFT_ARROW)
	{
		m_currentviewAngle += rotAm;
		vec3 distance = m_puppet->distanceFromOrigin;
		m_view = glm::translate(m_view, -m_currentviewDistance+m_distanceToPuppet);
		m_puppet->translate(-distance);
		m_view = glm::rotate(m_view, degreesToRadians(-rotAm), vec3(0,1.0f,0));
		m_puppet->rotate('y', rotAm);
		m_puppet->translate(distance);
		m_view = glm::translate(m_view, m_currentviewDistance-m_distanceToPuppet);
	}
	if(m_keypressed[RIGHT_ARROW] == RIGHT_ARROW)
	{
		m_currentviewAngle -= rotAm;
		vec3 distance = m_puppet->distanceFromOrigin;
		m_view = glm::translate(m_view, -m_currentviewDistance+m_distanceToPuppet);
		m_puppet->translate(-distance);
		m_view = glm::rotate(m_view, degreesToRadians(rotAm), vec3(0,1.0f,0));
		m_puppet->rotate('y', -rotAm);
		m_puppet->translate(distance);
		m_view = glm::translate(m_view, m_currentviewDistance-m_distanceToPuppet);
	}
	if(m_keypressed[DOWN_ARROW] == DOWN_ARROW)
	{
		m_currentviewDistance += vec3(-transAmBkw*sin(degreesToRadians(m_currentviewAngle)),0,-transAmBkw*cos(degreesToRadians(m_currentviewAngle)));
		m_view = glm::translate(m_view, vec3(-transAmBkw*sin(degreesToRadians(m_currentviewAngle)),0,-transAmBkw*cos(degreesToRadians(m_currentviewAngle))));
		m_puppet->translate(vec3(transAmBkw*sin(degreesToRadians(m_currentviewAngle)),0,transAmBkw*cos(degreesToRadians(m_currentviewAngle))));
		
		collision = checkCollision();
		if(collision)
		{
			float transAmBkwNeg = -transAmBkw;
			m_currentviewDistance += vec3(-transAmBkwNeg*sin(degreesToRadians(m_currentviewAngle)),0,-transAmBkwNeg*cos(degreesToRadians(m_currentviewAngle)));
			m_view = glm::translate(m_view, vec3(-transAmBkwNeg*sin(degreesToRadians(m_currentviewAngle)),0,-transAmBkwNeg*cos(degreesToRadians(m_currentviewAngle))));
			m_puppet->translate(vec3(transAmBkwNeg*sin(degreesToRadians(m_currentviewAngle)),0,transAmBkwNeg*cos(degreesToRadians(m_currentviewAngle))));
		}

		if(m_keypressed[UP_ARROW] == NO_KEY) m_char->runAnimation(m_currentviewAngle);
	}
}

void World3D::calculateTrajectoryHeight()
{
	float thetaRad = degreesToRadians(m_char->m_theta);
	float totalDistance = 3*abs((pow(m_char->m_yvelocity,2)*sin(2*thetaRad))/gravity);
	float velocitySign = 1;
	if(m_char->m_xstart >= totalDistance/2 && m_char->m_metric == 1)
	{
		m_char->m_metric = -1;
	}
	
	float distanceFromGround = m_puppet->distanceFromOrigin.y;
	m_puppet->translate(vec3(0,-distanceFromGround+m_char->startingHeight,0)); // translate back to the ground
	
	// check if the trajectory has ended, we've hit the ground
	if(distanceFromGround < 0)
	{
		m_char->refreshJump(m_currentviewAngle);
		return;
	}
	
	// let's calculate the new height of the trajectory at this given x coord
	float height = 0.3f*(m_char->m_xstart*tan(thetaRad) - (gravity*pow(m_char->m_xstart,2)/(2*pow(m_char->m_yvelocity*cos(thetaRad),2))));
	
	m_puppet->translate(vec3(0,height,0));
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void World3D::draw() {

	glEnable( GL_DEPTH_TEST );

	renderSceneGraph(*m_rootNode);
	
	glDisable( GL_DEPTH_TEST );
}

void World3D::render(const SceneNode * node)
{
	if (node->m_nodeType == NodeType::GeometryNode) {

		const GeometryNode * geometryNode = static_cast<const GeometryNode *>(node);

		updateShaderUniforms(m_shader, *geometryNode, m_view);


		// Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
		BatchInfo batchInfo = m_batchInfoMap[geometryNode->meshId];

		//-- Now render the mesh:
		m_shader.enable();
		glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
		m_shader.disable();

	}

	for (const SceneNode * child : node->children) {
		render(child);
	}
}

//----------------------------------------------------------------------------------------
void World3D::renderSceneGraph(const SceneNode & root) {

	// Bind the VAO once here, and reuse for all GeometryNode rendering below.
	glBindVertexArray(m_vao_meshData);

	for (const SceneNode * node : root.children) {

		render(node);
		
	}

	glBindVertexArray(0);
	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void World3D::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool World3D::cursorEnterWindowEvent (
		int entered
) {
	bool eventHandled(false);

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool World3D::mouseMoveEvent (
		double xPos,
		double yPos
) {
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {

		vec3 currPressPos(xPos,yPos,0);
		m_mousePressPos = currPressPos;
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool World3D::mouseButtonInputEvent (
		int button,
		int actions,
		int mods
) {
	bool eventHandled(false);

	if (actions == GLFW_PRESS && !ImGui::IsMouseHoveringAnyWindow()) {
		double xpos,ypos;
		glfwGetCursorPos(m_window, &xpos, &ypos);
		m_mousePressPos = vec3((float)xpos,(float)ypos,0);
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			m_pressed[LEFT] = LEFT;
		}
		if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
			m_pressed[MIDDLE] = MIDDLE;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			m_pressed[RIGHT] = RIGHT;
		}
	}
	if (actions == GLFW_RELEASE) {
		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			m_pressed[LEFT] = NONE;
		}
		if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
			m_pressed[MIDDLE] = NONE;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT) {
			m_pressed[RIGHT] = NONE;
		}
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool World3D::mouseScrollEvent (
		double xOffSet,
		double yOffSet
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool World3D::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);
	initPerspectiveMatrix();
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool World3D::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

	if( action == GLFW_PRESS ) {
		if(mods & GLFW_MOD_CONTROL)
		{
			m_keypressed[CTRL_KEY] = CTRL_KEY;
		}
		if( key == GLFW_KEY_M ) {
			show_gui = !show_gui;
			eventHandled = true;
		}
		if( key == GLFW_KEY_Q ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
		if( key == GLFW_KEY_C ) {
			m_celshading = !m_celshading;
		}
		if( key == GLFW_KEY_T ) {
			m_texturemapping = !m_texturemapping;
		}
		if( key == GLFW_KEY_B ) {
			m_bumpmapping = !m_bumpmapping;
		}
		if( key == GLFW_KEY_UP ) {
			m_keypressed[UP_ARROW] = UP_ARROW;
		}
		if( key == GLFW_KEY_LEFT ) {
			m_keypressed[LEFT_ARROW] = LEFT_ARROW;
		}
		if( key == GLFW_KEY_RIGHT ) {
			m_keypressed[RIGHT_ARROW] = RIGHT_ARROW;
		}
		if( key == GLFW_KEY_DOWN ) {
			m_keypressed[DOWN_ARROW] = DOWN_ARROW;
		}
		if( key == GLFW_KEY_SPACE ) {
			if(!m_char->m_midair && m_keypressed[UP_ARROW] == UP_ARROW)
			{
				m_char->refreshRun(m_currentviewAngle);
				characterJump();
				m_char->jumpAnimation(m_currentviewAngle); 
			}
		}
	}
	
	if( action == GLFW_RELEASE ) {
		if(mods & GLFW_MOD_CONTROL)
		{
			m_keypressed[CTRL_KEY] = NO_KEY;
		}
		if( key == GLFW_KEY_UP ) {
			m_keypressed[UP_ARROW] = NO_KEY;
			m_char->refreshRun(m_currentviewAngle);
		}
		if( key == GLFW_KEY_LEFT ) {
			m_keypressed[LEFT_ARROW] = NO_KEY;
		}
		if( key == GLFW_KEY_RIGHT ) {
			m_keypressed[RIGHT_ARROW] = NO_KEY;
		}
		if( key == GLFW_KEY_DOWN ) {
			m_keypressed[DOWN_ARROW] = NO_KEY;
			m_char->refreshRun(m_currentviewAngle);
		}
	}
	// Fill in with event handling code...

	return eventHandled;
}
