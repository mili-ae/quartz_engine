#pragma once

#include "light.hpp"
#include "omni_shadow_map.cpp"
#include <vector>

class PointLight : public Light {
public:
    glm::vec3 position;
    GLfloat farPlane; // How far away the camera can see

    PointLight();
    PointLight(GLuint shadowWidth, GLuint shadowHeight, GLfloat near, GLfloat far, glm::vec3 vcolor, GLfloat aIntensity, GLfloat dIntensity, glm::vec3 pos, GLfloat con, GLfloat lin, GLfloat exp);
    ~PointLight();

    void use(GLuint ambientIntesityLocation, GLuint colorLocation,
            GLuint diffuseIntensityLocation, GLuint positionLocation, 
            GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);
    
    std::vector<glm::mat4> calculateLightTransform();

protected:
    GLfloat constant, linear, exponent;
    
};
