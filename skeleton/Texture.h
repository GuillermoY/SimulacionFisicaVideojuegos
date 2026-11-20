#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/freeglut.h>

class Texture
{
public:
    Texture();
    ~Texture();

    bool Load(const std::string& path);
    void Bind() const;
    void Unbind() const;

    GLuint GetID() const;

private:
    GLuint m_textureID;
    bool m_loaded;
};

#endif