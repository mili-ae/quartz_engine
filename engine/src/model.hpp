#pragma once

#include <vector>
#include <string>

#include "../third_party/assimp/Importer.hpp"
#include "../third_party/assimp/scene.h"
#include "../third_party/assimp/postprocess.h"

#include "mesh.hpp"
#include "texture.hpp"

class Model {
public:
    Model();
    ~Model();
    void load(const std::string& filename);
    void render();
    void clear();

private:
    std::vector<Mesh*> meshes;
    std::vector<Texture*> textures;
    std::vector<unsigned int> meshToTex;
    std::string filePath;

    void loadNode(aiNode *node, const aiScene *scene);
    void loadMesh(aiMesh *mesh, const aiScene *scene);
    void loadMaterials(const aiScene *scene);
};
