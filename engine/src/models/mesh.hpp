#pragma once

#include "../../third_party/glew/glew.h"
#include "quartz_lib.hpp"

class Mesh
{
    GLuint VAO, VBO, EBO;
    GLsizei indexCount;

public:
    Mesh();
    ~Mesh();
    EXPORT_FN void create(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
    EXPORT_FN void render();
    EXPORT_FN void clear();
};