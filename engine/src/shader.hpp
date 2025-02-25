#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include "../third_party/glew/glew.h"
#include "../third_party/glm/glm.hpp"
#include "../third_party/glm/gtc/type_ptr.hpp"

#include "common.hpp"
#include "lighting/directional_light.hpp"
#include "lighting/point_light.hpp"
#include "lighting/spot_light.hpp"

class Shader
{
    GLuint shaderID;
    void compile(const char* vertexCode, const char* fragmentCode);
    void compile(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
    void compileProgram();
    void add(GLuint prog, const char* code, GLenum shaderType);
    std::string readFile(const char* path);
public:
    GLuint uProjection, uModel, uView, uCameraPosition,
        uSpecularIntensity, uShininess,
        uTexture, uDirectionalLightTransform, uDirectionalShadowMap,
        uOmniLightPos, uFarPlane;

    GLuint uLightMatrices[6];
    
    int pointLightCount;
    int spotLightCount;

    struct {
        GLuint uColor;
        GLuint uAmbientIntensity;
        GLuint uDiffuseIntensity;

        GLuint uDirection;
    } uDirectionalLight;

    GLuint uPointLightCount;
    GLuint uSpotLightCount;

    struct {
        GLuint uColor;
        GLuint uAmbientIntensity;
        GLuint uDiffuseIntensity;

        GLuint uPosition;
        GLuint uConstant;
        GLuint uLinear;
        GLuint uExponent;
    } uPointLight[MAX_POINT_LIGHTS];

    struct {
        GLuint uColor;
        GLuint uAmbientIntensity;
        GLuint uDiffuseIntensity;

        GLuint uPosition;
        GLuint uConstant;
        GLuint uLinear;
        GLuint uExponent;

        GLuint uDirection;
        GLuint uEdge;
    } uSpotLight[MAX_SPOT_LIGHTS];

    struct {
        GLuint shadowMap;
        GLuint farPlane;
    } uOmniShadowMap[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

    Shader();
    ~Shader();

    void createFromString(const char* vertexCode, const char* fragmentCode);
    void createFromFile(const char* vertexPath, const char* fragmentPath);
    void createFromFile(const char* vertexPath, const char* geometryPath, const char* fragmentPath);
    void validate();
    void setDirectionalLight(DirectionalLight *dLight);
    void setPointLights(PointLight *pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
    void setSpotLights(SpotLight *sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset);
    void setTexture(GLuint textureUnit);
    void setDirectionalShadowMap(GLuint textureUnit);
    void setDirectionalLightTransform(glm::mat4* lTransform);
    void setLightMatrices(std::vector<glm::mat4> lightMatrices);

    void use();
    void clear();
};