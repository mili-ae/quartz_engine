#pragma once

#include "models/model.hpp"
#include "models/mesh.hpp"
#include "models/material.hpp"
#include "texture.hpp"
#include "transform.cpp"
#include "common.hpp"

#include "../third_party/glm/glm.hpp"
#include "../third_party/glm/gtc/type_ptr.hpp"

#include <functional>

class GameObject
{
public:
    GameObject();
    ~GameObject();

    void update(GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess);
    void render();

    inline void addModel(Model *model) { _model = model; }
    inline void addMesh(Mesh *mesh) { _mesh = mesh; }
    inline void addMaterial(Material mat) { _material = mat; }

    void addTexture(std::string path);

    inline void setUpdate(std::function<void()> func) { _update = func; }

    Transform transform;
    
private:
    Model *_model;
    Mesh *_mesh;
    Texture _texture;
    std::function<void()> _update;
    Material _material;
};