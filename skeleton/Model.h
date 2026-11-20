
#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <map>

#include "Mesh.h"

#include <assimp/scene.h>

class Model
{
public:
    Model() = default;
    ~Model();

    bool Load(const std::string& path);

    void Draw() const;

    bool IsLoaded() const
    {
        return m_loaded;
    }

private:
    std::vector<Mesh> m_meshes;

    // Guardamos las texturas para liberarlas al destruir el modelo.
    std::vector<Texture*> m_textures;

    std::map<std::string, Texture*> m_loadedTextures;

    std::string m_directory;

    bool m_loaded = false;

    void ProcessNode(
        const aiNode* node,
        const aiScene* scene,
        const aiMatrix4x4& parentTransform
    );

    Mesh ProcessMesh(
        const aiMesh* mesh,
        const aiScene* scene,
        const aiMatrix4x4& nodeTransform
    );

    Texture* LoadMaterialTexture(
        const aiMaterial* material,
        aiTextureType type
    );
};

#endif
