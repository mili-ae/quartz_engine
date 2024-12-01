#include "directional_light.hpp"

DirectionalLight::DirectionalLight() : Light()
{
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(glm::vec3 vcolor, GLfloat aIntensity, GLfloat dIntensity, glm::vec3 dir) : Light(vcolor, aIntensity, dIntensity), direction(dir)
{
}

void DirectionalLight::use(GLuint ambientIntesityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation)
{
    glUniform3f(colorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntesityLocation, ambientIntensity);

    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

DirectionalLight::~DirectionalLight()
{
}