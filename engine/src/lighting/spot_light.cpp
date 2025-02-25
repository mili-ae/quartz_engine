#include "spot_light.hpp"

SpotLight::SpotLight() : PointLight()
{
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    edge = 0.0f;
    procEdge = cosf(glm::radians(edge));
    isOn = true;
}

SpotLight::SpotLight(GLuint shadowWidth, GLuint shadowHeight, GLfloat near, GLfloat far, glm::vec3 vcolor, GLfloat aIntensity, GLfloat dIntensity, glm::vec3 pos, glm::vec3 dir, GLfloat con, GLfloat lin, GLfloat exp, GLfloat edg)
                : PointLight(shadowWidth, shadowHeight, near, far, vcolor, aIntensity, dIntensity, pos, con, lin, exp), edge(edg)
{
    direction = glm::normalize(dir);
    procEdge = cosf(glm::radians(edge));
}

SpotLight::~SpotLight()
{
}

void SpotLight::use(GLuint ambientIntesityLocation, GLuint colorLocation,
            GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
            GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation, GLuint edgeLocation)
{
    glUniform3f(colorLocation, color.x, color.y, color.z);
    if (isOn)
    {
        glUniform1f(ambientIntesityLocation, ambientIntensity);
        glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    }
    else
    {
        glUniform1f(ambientIntesityLocation, 0.0f);
    glUniform1f(diffuseIntensityLocation, 0.0f);
    }

    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);

    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(edgeLocation, procEdge);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	position = pos;
	direction = dir;
}