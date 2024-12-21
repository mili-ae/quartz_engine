#pragma once

#include <vector>
#include <string>

#include "../third_party/assimp/Importer.hpp"
#include "../third_party/assimp/scene.h"
#include "../third_party/assimp/postprocess.h"

#include "mesh.hpp"
#include "texture.hpp"
#include "transform.cpp"

class Model {
public:
    Model();
    Model(const std::string& filename);
    ~Model();
    void loadMesh(const std::string& filename);
    void setMaterial(Material mat) { material = mat; }
    void render();
    void clear();

    void translate(glm::vec3 pos);
    void rotate(float angle, glm::vec3 axis);
    void scale(glm::vec3 s);
    void setModel() { transform.matrixModel = glm::mat4(1.0f);};
    glm::mat4 getModel() { return transform.matrixModel; }

    Material material;

private:
    std::vector<Mesh*> meshes;
    std::vector<Texture*> textures;
    std::vector<unsigned int> meshToTex;
    std::string filePath;
    
    Transform transform;
    
    void loadAiNode(aiNode *node, const aiScene *scene);
    void loadAiMesh(aiMesh *mesh, const aiScene *scene);
    void loadAiMaterials(const aiScene *scene);
};
