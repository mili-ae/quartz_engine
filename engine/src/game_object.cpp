#include "game_object.hpp"

GameObject::GameObject()
{
    _update = nullptr;
    _model = nullptr;
    _mesh = nullptr;
    transform = Transform();
}

GameObject::~GameObject()
{
    if (_model != nullptr)
    {
        _model->clear();
    }

    if (_mesh != nullptr)
    {
        _mesh->clear();
    }
}

void GameObject::update(GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess)
{
    transform.matrixModel = glm::mat4(1.0f);
    _update();

    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(transform.matrixModel));
    _texture.use();
    _material.use(uniformSpecularIntensity, uniformShininess);
}

void GameObject::render()
{
    if (_model != nullptr)
    {
        _model->render();
    }

    if (_mesh != nullptr)
    {
        _mesh->render();
    }
}

void GameObject::addTexture(std::string path)
{
    _texture = Texture(path.c_str());
    _texture.load();
}