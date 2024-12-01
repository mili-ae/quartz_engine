#include "light.hpp"

Light::Light()
{
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 1.0f;

    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    diffuseIntensity = 0.0f;
}

Light::Light(glm::vec3 vcolor, GLfloat aIntensity, glm::vec3 dir, GLfloat dIntensity)
{
    color = vcolor;
    ambientIntensity = aIntensity;

    direction = dir;
    diffuseIntensity = dIntensity;
}

void Light::use(GLuint ambientIntesityLocation, GLuint colorLocation, GLuint diffuseIntensityLocation, GLuint directionLocation)
{
    glUniform3f(colorLocation, color.x, color.y, color.z);
    glUniform1f(ambientIntesityLocation, ambientIntensity);

    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

Light::~Light()
{
}
