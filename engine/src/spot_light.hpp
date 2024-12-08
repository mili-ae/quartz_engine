#pragma once

#include "point_light.hpp"

class SpotLight : public PointLight {
public:
    SpotLight();
    SpotLight(GLuint shadowWidth, GLuint shadowHeight, GLfloat near, GLfloat far, glm::vec3 vcolor, GLfloat aIntensity, GLfloat dIntensity, glm::vec3 pos, glm::vec3 dir, GLfloat con, GLfloat lin, GLfloat exp, GLfloat edg);
    ~SpotLight();

    void use(GLuint ambientIntesityLocation, GLuint colorLocation,
            GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
            GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint edgeLocation);
    void SetFlash(glm::vec3 pos, glm::vec3 dir);
    void toggle() { isOn = !isOn; }

private:
    glm::vec3 direction;
    bool isOn;
    GLfloat edge, procEdge;
};
