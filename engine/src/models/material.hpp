#pragma once

#include "../../third_party/glew/glew.h"

class Material {
public:
    Material();
    Material(GLfloat sIntensity, GLfloat shine);
    ~Material();

    void use(GLuint specularLocation, GLuint shininessLocation);

private:
    GLfloat specularIntensity; // How much specular light should be on the object
    GLfloat shininess; // How smooth the surface is
};
