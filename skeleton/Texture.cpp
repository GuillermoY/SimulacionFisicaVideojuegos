#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

Texture::Texture()
    : m_textureID(0), m_loaded(false)
{
}

Texture::~Texture()
{
    if (m_textureID != 0)
        glDeleteTextures(1, &m_textureID);
}

bool Texture::Load(const std::string& path)
{
    int width, height, channels;

    //stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(
        path.c_str(),
        &width,
        &height,
        &channels,
        0
    );

    if (!data)
    {
        std::cout << "Error cargando textura: "
            << path << std::endl;

        std::cout << "STB: "
            << stbi_failure_reason() << std::endl;

        return false;
    }

    GLenum format = GL_RGB;
    GLenum internalFormat = GL_RGB8;

    if (channels == 1)
    {
        format = GL_RED;
        internalFormat = GL_RED;
    }
    else if (channels == 3)
    {
        format = GL_RGB;
        internalFormat = GL_RGB8;
    }
    else if (channels == 4)
    {
        format = GL_RGBA;
        internalFormat = GL_RGBA8;
    }
    else
    {
        stbi_image_free(data);
        return false;
    }

    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );
    GLenum error = glGetError();

    GLint textureWidth = 0;
    GLint textureHeight = 0;

    glGetTexLevelParameteriv(
        GL_TEXTURE_2D,
        0,
        GL_TEXTURE_WIDTH,
        &textureWidth
    );

    glGetTexLevelParameteriv(
        GL_TEXTURE_2D,
        0,
        GL_TEXTURE_HEIGHT,
        &textureHeight
    );
    //glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    m_loaded = true;

    //std::cout << "Textura cargada: " << path << std::endl;
    //std::cout << "ID: " << m_textureID << std::endl;
    //std::cout << "Width: " << width << std::endl;
    //std::cout << "Height: " << height << std::endl;
    //std::cout << "Channels: " << channels << std::endl;

    return true;
}

void Texture::Bind() const
{
    if (m_loaded)
        glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::GetID() const
{
    return m_textureID;
}