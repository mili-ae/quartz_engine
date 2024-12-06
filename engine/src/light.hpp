#pragma once

#include "../third_party/glew/glew.h"
#include "../third_party/glm/gtc/matrix_transform.hpp"
#include "../third_party/glm/glm.hpp"

#include "shadow_map.cpp"

class Light {
public:
    ShadowMap* shadowMap;

    Light();
    Light(GLuint shadowWidth, GLuint shadowHeight, glm::vec3 vcolor, GLfloat aIntensity, GLfloat dIntensity);
    ~Light();

    void use(GLuint ambientIntesityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation);
protected:
    glm::vec3 color;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;

    glm::mat4 lightProj;
};