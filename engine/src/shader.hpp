#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include "../third_party/glew/glew.h"

class Shader
{
    GLuint shaderID;
    void compile(const char* vertexCode, const char* fragmentCode);
    void add(GLuint prog, const char* code, GLenum shaderType);
    std::string readFile(const char* path);
public:
    GLuint uProjection, uModel;
    Shader();
    ~Shader();

    void createFromString(const char* vertexCode, const char* fragmentCode);
    void createFromFile(const char* vertexPath, const char* fragmentPath);
    void use();
    void clear();
};