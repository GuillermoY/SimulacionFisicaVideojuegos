
#include "Mesh.h"

#include <GL/freeglut.h>
#include <iostream>

Mesh::Mesh(
    std::string m_codeName,
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices,
    Texture* texture,
    const physx::PxVec3& color
)
    : m_codeName(m_codeName),
    m_vertices(vertices),
    m_indices(indices),
    m_texture(texture),
    m_color(color)
{
}

void Mesh::Draw() const
{

    GLfloat diffuse[] = {
        m_color.x,
        m_color.y,
        m_color.z,
        1.0f
    };

    GLfloat ambient[] = {
        m_color.x * 0.3f,
        m_color.y * 0.3f,
        m_color.z * 0.3f,
        1.0f
    };

    glDisable(GL_COLOR_MATERIAL);

    glMaterialfv(
        GL_FRONT_AND_BACK,
        GL_DIFFUSE,
        diffuse
    );

    glMaterialfv(
        GL_FRONT_AND_BACK,
        GL_AMBIENT,
        ambient
    );

    glEnable(GL_NORMALIZE);

    if (m_texture)
    {
        glEnable(GL_TEXTURE_2D);

        glTexEnvi(
            GL_TEXTURE_ENV,
            GL_TEXTURE_ENV_MODE,
            GL_MODULATE
        );

        m_texture->Bind();
        GLint boundTexture = 0;

        glGetIntegerv(
            GL_TEXTURE_BINDING_2D,
            &boundTexture
        );
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
    }

    glBegin(GL_TRIANGLES);

    for (unsigned int index : m_indices)
    {
        const Vertex& vertex = m_vertices[index];

        glNormal3f(
            vertex.Normal.x,
            vertex.Normal.y,
            vertex.Normal.z
        );

        glTexCoord2f(
            vertex.TexCoords.x,
            vertex.TexCoords.y
        );

        glVertex3f(
            vertex.Position.x,
            vertex.Position.y,
            vertex.Position.z
        );
    }

    glEnd();

    if (m_texture)
    {
        m_texture->Unbind();
    }

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_COLOR_MATERIAL);
}
