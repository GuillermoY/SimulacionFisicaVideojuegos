
#pragma once
#ifndef MESH_H
#define MESH_H

#include <vector>
#include "PxPhysicsAPI.h"
#include "Texture.h"

struct Vertex
{
    physx::PxVec3 Position;
    physx::PxVec3 Normal;
    physx::PxVec2 TexCoords;
};

class Mesh
{
public:
    Mesh(
        std::string m_codeName,
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices,
        Texture* texture,
        const physx::PxVec3& color
    );

    void Draw() const;

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;

    Texture* m_texture;
    physx::PxVec3 m_color;
    std::string m_codeName;
};

#endif
