#include "point_light.hpp"

PointLight::PointLight() : Light()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    constant = 1.0f;
    linear = 0.0f;
    exponent = 0.0f;
}
PointLight::PointLight(GLuint shadowWidth, GLuint shadowHeight, GLfloat near, GLfloat far, glm::vec3 vcolor, GLfloat aIntensity, GLfloat dIntensity, glm::vec3 pos, GLfloat con, GLfloat lin, GLfloat exp) 
            : Light(shadowWidth, shadowHeight, vcolor, aIntensity, dIntensity), position(pos), constant(con), linear(lin), exponent(exp), farPlane(far)
{
    float aspect = (float)shadowWidth / (float)shadowHeight;
    lightProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

    shadowMap = new OmniShadowMap();
    shadowMap->init(shadowWidth, shadowHeight);
}

PointLight::~PointLight()
{
}

void PointLight::use(GLuint ambientIntesityLocation, GLuint colorLocation,
            GLuint diffuseIntensityLocation, GLuint positionLocation, 
            GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
    glUniform3f(colorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntesityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);

    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
}

std::vector<glm::mat4> PointLight::calculateLightTransform()
{
    std::vector<glm::mat4> lightMatrices;
    // +x, -x
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));

    // +y, -y
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));

    // +z, -z
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
    lightMatrices.push_back(lightProj * glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

    return lightMatrices;
}