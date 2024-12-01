#pragma once

#include "light.hpp"

class PointLight : public Light {
public:
    PointLight();
    PointLight(glm::vec3 vcolor, GLfloat aIntensity, GLfloat dIntensity, glm::vec3 pos, GLfloat con, GLfloat lin, GLfloat exp);
    ~PointLight();

    void use(GLuint ambientIntesityLocation, GLuint colorLocation,
            GLuint diffuseIntensityLocation, GLuint positionLocation, 
            GLfloat constantLocation, GLfloat linearLocation, GLfloat exponentLocation);

private:
    glm::vec3 position;

    GLfloat constant, linear, exponent;
};
