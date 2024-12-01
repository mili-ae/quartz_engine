#pragma once

#include "../third_party/glew/glew.h"
#include "../third_party/glm/glm.hpp"

class Light {
public:
    Light();
    Light(glm::vec3 vcolor, GLfloat aIntensity, glm::vec3 dir, GLfloat dIntensity);
    ~Light();

    void use(GLuint ambientIntesityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation);
private:
    glm::vec3 color;
    GLfloat ambientIntensity;

    glm::vec3 direction;
    GLfloat diffuseIntensity;
};