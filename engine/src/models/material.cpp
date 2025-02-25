#include "material.hpp"

Material::Material() {
    specularIntensity = 09.0f;
    shininess = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine) 
        : specularIntensity(sIntensity), shininess(shine)
{
}

Material::~Material()
{
}

void Material::use(GLuint specularLocation, GLuint shininessLocation)
{
    glUniform1f(specularLocation, specularIntensity);
    glUniform1f(shininessLocation, shininess);
}