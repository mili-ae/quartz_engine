#include "model.hpp"

Model::Model()
{
}

Model::~Model()
{
}

void Model::load(const std::string& filename)
{
    // int idx = std::string(filename).rfind("\\");
    // filePath = std::string(filename).substr(idx + 1);
    filePath = filename.substr(0, filename.find_last_of("\\/"));

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if (!scene)
    {
        printf("Model (%s) failed to load: %s", filename.c_str(), importer.GetErrorString());
        return;
    }

    loadNode(scene->mRootNode, scene);
    loadMaterials(scene);
}

void Model::render()
{
    for (size_t i = 0; i < meshes.size(); i++)
    {
        unsigned int materialIndex = meshToTex[i];

        if (materialIndex < textures.size() && textures[materialIndex])
        {
            textures[materialIndex]->use();
        }

        meshes[i]->render();
    }
}

void Model::clear()
{
    for (size_t i = 0; i < meshes.size(); i++)
    {
        if (meshes[i])
        {
            delete meshes[i];
            meshes[i] = nullptr;
        }
    }

    for (size_t i = 0; i < textures.size(); i++)
    {
        if (textures[i])
        {
            delete textures[i];
            textures[i] = nullptr;
        }
    }
}

void Model::loadNode(aiNode *node, const aiScene *scene)
{
    for (size_t i = 0; i < node->mNumMeshes; i++)
    {
        loadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }

    for (size_t i = 0; i < node->mNumChildren; i++)
    {
        loadNode(node->mChildren[i], scene);
    }
}

void Model::loadMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        // Vertices
        vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });

        // Texture
        if (mesh->mTextureCoords[0])
        {
            vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
        } else {
            vertices.insert(vertices.end(), { 0.0f, 0.0f });
        }

        // Normals
        vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
    }

    // Indices
    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh *newMesh = new Mesh();
    newMesh->create(&vertices[0], &indices[0], vertices.size(), indices.size());
    meshes.push_back(newMesh);
    meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::loadMaterials(const aiScene *scene)
{
    textures.resize(scene->mNumMaterials);
    
    for(size_t i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial *material = scene->mMaterials[i];
        textures[i] = nullptr;

        if (material->GetTextureCount(aiTextureType_DIFFUSE))
        {
            aiString path;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            {
                // Clean mess in model export with filepaths
                int idx = std::string(path.data).rfind("\\");
                std::string fileName = std::string(path.data).substr(idx + 1);

                // std::string texPath = std::string("Textures/") + fileName;
                std::string texPath = filePath +"/" + fileName;
                textures[i] = new Texture(texPath.c_str());

                if (!textures[i]->load())
                {
                    printf("Failed to load texture at: %s\n", texPath.c_str());
                    delete textures[i];
                    textures[i] = nullptr;
                }
            }
        }

        if (!textures[i])
        {
            textures[i] = new Texture("assets/textures/plain.png");
            textures[i]->load();
        }
    }
}