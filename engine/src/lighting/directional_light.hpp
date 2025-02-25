#pragma once

#include "light.cpp"

class DirectionalLight : public Light {
public:
    DirectionalLight();
    DirectionalLight(GLuint shadowWidth, GLuint shadowHeight, glm::vec3 vcolor, GLfloat aIntensity, GLfloat dIntensity, glm::vec3 dir);
    ~DirectionalLight();

    void use(GLuint ambientIntesityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation);

    glm::mat4 calculateLightTransform();

private:
    glm::vec3 direction;
};
