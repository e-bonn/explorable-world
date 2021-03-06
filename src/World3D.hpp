#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/MeshConsolidator.hpp"

#include "SceneNode.hpp"
#include "Character.hpp"
#include "DynamicObject.hpp"
#include "Texture.hpp"

#include <time.h>
#include <glm/glm.hpp>
#include <memory>
#include <stack>

struct LightSource {
	glm::vec3 position;
	glm::vec3 rgbIntensity;
};

enum MousePress {
	LEFT,
	MIDDLE,
	RIGHT,
	NONE
};

enum KeyPress {
	LEFT_ARROW,
	UP_ARROW,
	DOWN_ARROW,
	RIGHT_ARROW,
	CTRL_KEY,
	NO_KEY
};


class GeometryNode;

class World3D : public CS488Window {
public:
	World3D(const std::string & luaSceneFile);
	virtual ~World3D();

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	//-- Virtual callback methods
	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

	//-- One time initialization methods:
	void processLuaSceneFile(const std::string & filename);
	void createShaderProgram();
	void enableVertexShaderInputSlots();
	void uploadVertexDataToVbos(const MeshConsolidator & meshConsolidator);
	void mapVboDataToVertexShaderInputLocations();
	
	void initViewMatrix();
	void initLightSources();
	void initShadowData();
	void updateShaderUniforms(const ShaderProgram & shader,const GeometryNode & node,const glm::mat4 & viewMatrix);

	void initPerspectiveMatrix();
	void uploadCommonSceneUniforms();
	void renderSceneGraph(const SceneNode &node);

	void render(const SceneNode * node);
	
	bool checkCollision();
	void moveCamera();
	void moveCharacter();
	void characterJump();
	void calculateTrajectoryHeight();

	bool m_celshading;
	bool m_texturemapping;
	bool m_bumpmapping;

	glm::mat4 m_perpsective;
	glm::mat4 m_view;
	glm::mat4 m_model;

	LightSource m_light;
	
	time_t m_beginTime;

	glm::vec3 m_mousePressPos;
	glm::vec3 m_projectedMousePressPos;
	MousePress m_pressed[3];
	KeyPress m_keypressed[5];
	
	float m_currentviewAngle;
	glm::vec3 m_currentviewDistance;
	glm::vec3 m_distanceToPuppet;

	//-- GL resources for mesh geometry data:
	GLuint m_vao_meshData;
	GLuint m_vbo_vertexPositions;
	GLuint m_vbo_vertexNormals;
	GLint m_positionAttribLocation;
	GLint m_normalAttribLocation;
	ShaderProgram m_shader;

	//-- GL resources for trackball circle geometry:
	GLuint m_vbo_arcCircle;
	GLuint m_vao_arcCircle;
	GLint m_arc_positionAttribLocation;
	ShaderProgram m_shader_arcCircle;

	// BatchInfoMap is an associative container that maps a unique MeshId to a BatchInfo
	// object. Each BatchInfo object contains an index offset and the number of indices
	// required to render the mesh with identifier MeshId.
	BatchInfoMap m_batchInfoMap;

	std::string m_luaSceneFile;

	std::shared_ptr<SceneNode> m_rootNode;
	SceneNode *m_puppet;
	SceneNode *m_world;
	SceneNode *m_movingDoor;
	Character *m_char;
	std::vector<DynamicObject*> m_dobjects;

	// textures //
	Texture *grass;
	Texture *stonehouse;
	Texture *wood;
	Texture *soil;
	Texture *metal;
	Texture *treeneedles;
	Texture *gravel;

	Texture *gravel_normalmap;
};
