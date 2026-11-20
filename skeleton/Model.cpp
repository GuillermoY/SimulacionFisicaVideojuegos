
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <filesystem>

#include <iostream>

Model::~Model()
{
    for (Texture* texture : m_textures)
    {
        delete texture;
    }

    m_textures.clear();
}

bool Model::Load(const std::string& path)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices
    );

    if (!scene || !scene->mRootNode ||
        (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
    {
        std::cout << "Error Assimp: "
            << importer.GetErrorString() << std::endl;

        return false;
    }

    size_t separator = path.find_last_of("/\\");

    if (separator != std::string::npos)
        m_directory = path.substr(0, separator);
    else
        m_directory = ".";

    ProcessNode(scene->mRootNode, scene, aiMatrix4x4());

    m_loaded = true;

    return true;
}

void Model::ProcessNode(const aiNode* node, const aiScene* scene, const aiMatrix4x4& parentTransform)
{
    aiMatrix4x4 globalTransform =
        parentTransform * node->mTransformation;

    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        m_meshes.push_back(
            ProcessMesh(mesh, scene, globalTransform)
        );
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(
            node->mChildren[i],
            scene,
            globalTransform
        );
    }
}

Mesh Model::ProcessMesh(const aiMesh* mesh, const aiScene* scene, const aiMatrix4x4& nodeTransform)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        // 1. Multiplicamos el vértice por la matriz de transformación local del nodo
        aiVector3D localPos = nodeTransform * mesh->mVertices[i];

        vertex.Position = physx::PxVec3(
            localPos.x,
            localPos.y,
            localPos.z
        );

        if (mesh->HasNormals())
        {
            // Las normales solo se rotan, no se trasladan
            aiMatrix3x3 normalMatrix(nodeTransform);
            aiVector3D localNormal = normalMatrix * mesh->mNormals[i];
            localNormal.Normalize();

            vertex.Normal = physx::PxVec3(
                localNormal.x,
                localNormal.y,
                localNormal.z
            );
        }
        else
        {
            vertex.Normal = physx::PxVec3(0, 1, 0);
        }

        if (mesh->mTextureCoords[0])
        {
            vertex.TexCoords = physx::PxVec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        }
        else
        {
            vertex.TexCoords = physx::PxVec2(0, 0);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace& face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    Texture* texture = nullptr;

    aiColor3D diffuseColor(1.0f, 1.0f, 1.0f);

    if (mesh->mMaterialIndex < scene->mNumMaterials)
    {
        aiMaterial* material =
            scene->mMaterials[mesh->mMaterialIndex];

        // Obtener color del material (.mtl)
        material->Get(
            AI_MATKEY_COLOR_DIFFUSE,
            diffuseColor
        );

        // Obtener textura si existe
        texture = LoadMaterialTexture(
            material,
            aiTextureType_DIFFUSE
        );
    }

    physx::PxVec3 color(
        diffuseColor.r,
        diffuseColor.g,
        diffuseColor.b
    );

    std::string collectionName = mesh->mName.length > 0
        ? mesh->mName.C_Str()
        : "nodeName";


    return Mesh(collectionName, vertices, indices, texture, color);
}

Texture* Model::LoadMaterialTexture(
    const aiMaterial* material,
    aiTextureType type
)
{
    if (material->GetTextureCount(type) == 0)
        return nullptr;

    aiString texturePath;

    if (material->GetTexture(type, 0, &texturePath) != AI_SUCCESS)
        return nullptr;

    std::string path = texturePath.C_Str();

    // De momento no soportamos texturas incrustadas.
    if (!path.empty() && path[0] == '*')
    {
        std::cout << "Textura incrustada no soportada: "
            << path << std::endl;

        return nullptr;
    }

    std::filesystem::path textureFile(path);

    if (textureFile.is_relative())
    {
        textureFile = std::filesystem::path(m_directory) / textureFile;
    }

    std::string fullPath = textureFile.lexically_normal().string();

    auto it = m_loadedTextures.find(fullPath);

    if (it != m_loadedTextures.end())
        return it->second;

    Texture* texture = new Texture();

    if (!texture->Load(fullPath))
    {
        delete texture;
        return nullptr;
    }

    m_textures.push_back(texture);
    m_loadedTextures[fullPath] = texture;

    return texture;
}

void Model::Draw() const
{
    if (!m_loaded)
        return;

    for (const Mesh& mesh : m_meshes)
    {
        mesh.Draw();
    }
}
