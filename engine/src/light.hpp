#pragma once

#include "../third_party/glew/glew.h"
#include "../third_party/glm/glm.hpp"

class Light {
public:
    Light();
    Light(glm::vec3 vcolor, GLfloat aIntensity, GLfloat dIntensity);
    ~Light();

    void use(GLuint ambientIntesityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation);
protected:
    glm::vec3 color;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;
};