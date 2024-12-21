#include "transform.hpp"

Transform::Transform()
{
    matrixModel = glm::mat4(1.0f);
    position = glm::vec3(0, 0, 0);
    rotation = glm::vec3(0, 0, 0);
    scale = glm::vec3(0, 0, 0);
}

Transform::~Transform()
{
}