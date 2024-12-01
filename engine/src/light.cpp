#include "light.hpp"

Light::Light()
{
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 1.0f;
    diffuseIntensity = 0.0f;
}

Light::Light(glm::vec3 vcolor, GLfloat aIntensity, GLfloat dIntensity)
{
    color = vcolor;
    ambientIntensity = aIntensity;
    diffuseIntensity = dIntensity;
}

Light::~Light()
{
}
