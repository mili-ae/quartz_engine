#include "light.hpp"

Light::Light()
{
    color = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 1.0f;
    diffuseIntensity = 0.0f;
}

Light::Light(GLuint shadowWidth, GLuint shadowHeight, glm::vec3 vcolor, GLfloat aIntensity, GLfloat dIntensity)
                : color(vcolor), ambientIntensity(aIntensity), diffuseIntensity(dIntensity)
{
    shadowMap = new ShadowMap();
    shadowMap->init(shadowWidth, shadowHeight);
}

Light::~Light()
{
}
