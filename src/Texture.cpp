#include "Texture.hpp"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include <SOIL/SOIL.h>

#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"

using namespace std;
using namespace glm;

Texture::Texture(std::string texturePath)
	: m_texturePath(texturePath), m_imgWidth(0), m_imgHeight(0), m_textureId(0)
{
}

Texture::~Texture()
{
}

void Texture::load()
{
    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    unsigned char* image = SOIL_load_image(m_texturePath.c_str(), &m_imgWidth, &m_imgHeight, 0, SOIL_LOAD_RGB);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_imgWidth, m_imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}