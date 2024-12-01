#pragma once

#include "light.cpp"

class DirectionalLight : public Light {
public:
    DirectionalLight();
    DirectionalLight(glm::vec3 vcolor, GLfloat aIntensity, GLfloat dIntensity, glm::vec3 dir);
    ~DirectionalLight();

    void use(GLuint ambientIntesityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation);

private:
    glm::vec3 direction;
};
