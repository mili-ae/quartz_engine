#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include "../third_party/glew/glew.h"

#include "common.hpp"
#include "directional_light.hpp"
#include "point_light.hpp"

class Shader
{
    GLuint shaderID;
    void compile(const char* vertexCode, const char* fragmentCode);
    void add(GLuint prog, const char* code, GLenum shaderType);
    std::string readFile(const char* path);
public:
    GLuint uProjection, uModel, uView, uCameraPosition,
        uSpecularIntensity, uShininess;
    
    int pointLightCount;

    struct {
        GLuint uColor;
        GLuint uAmbientIntensity;
        GLuint uDiffuseIntensity;

        GLuint uDirection;
    } uDirectionalLight;

    GLuint uPointLightCount;

    struct {
        GLuint uColor;
        GLuint uAmbientIntensity;
        GLuint uDiffuseIntensity;

        GLuint uPosition;
        GLuint uConstant;
        GLuint uLinear;
        GLuint uExponent;
    } uPointLight[MAX_POINT_LIGHTS];

    Shader();
    ~Shader();

    void createFromString(const char* vertexCode, const char* fragmentCode);
    void createFromFile(const char* vertexPath, const char* fragmentPath);
    void setDirectionalLight(DirectionalLight *dLight);
    void setPointLights(PointLight *pLight, unsigned int lightCount);
    void use();
    void clear();
};