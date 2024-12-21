#pragma once

#include "../third_party/glm/gtc/matrix_transform.hpp"
#include "../third_party/glm/glm.hpp"

class Transform {
public:
    Transform();
    ~Transform();

    glm::mat4 matrixModel;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

private:
    

};
