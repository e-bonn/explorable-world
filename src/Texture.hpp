#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/MeshConsolidator.hpp"

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Texture {

	public:
	Texture(std::string texturePath);
	~Texture();

	void load();

	GLuint m_textureId;
	std::string m_texturePath;
	int m_imgWidth;
	int m_imgHeight;
};