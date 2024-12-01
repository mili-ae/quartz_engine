#include "shader.hpp"

Shader::Shader()
{
    shaderID = 0;
    uModel = 0;
    uView = 0;
    uProjection = 0;

    pointLightCount = 0;
}
Shader::~Shader()
{
    clear();
}

void Shader::createFromString(const char* vertexCode, const char* fragmentCode)
{
    compile(vertexCode, fragmentCode);
}

void Shader::createFromFile(const char* vertexPath, const char* fragmentPath)
{
    compile(readFile(vertexPath).c_str(), readFile(fragmentPath).c_str());
}

void Shader::setDirectionalLight(DirectionalLight *dLight)
{
    dLight->use(uDirectionalLight.uAmbientIntensity, uDirectionalLight.uColor, uDirectionalLight.uDiffuseIntensity, uDirectionalLight.uDirection);
}

void Shader::setPointLights(PointLight *pLight, unsigned int lightCount)
{
    if (lightCount > MAX_POINT_LIGHTS)
    {
        lightCount = MAX_POINT_LIGHTS;
    }

    glUniform1i(uPointLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++)
    {
        pLight[i].use(uPointLight[i].uAmbientIntensity, uPointLight[i].uColor, 
                        uPointLight[i].uDiffuseIntensity, uPointLight[i].uPosition, 
                        uPointLight[i].uConstant, uPointLight[i].uLinear, uPointLight[i].uExponent);
    }
}

void Shader::use()
{
    if (shaderID == 0) return;
    glUseProgram(shaderID);
}

void Shader::clear()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }

    uModel = 0;
    uProjection = 0;
}

std::string Shader::readFile(const char* path)
{
    std::string content;
    std::ifstream fileStream(path, std::ios::in);

    if (!fileStream.is_open())
    {
        printf("Failed to read %s! File does not exist", path);
        return "";
    }

    std::string line = "";
    while(!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

void Shader::compile(const char* vertexCode, const char* fragmentCode)
{
    shaderID = glCreateProgram();

    if (!shaderID)
    {
        printf("Error creating shader program\n");
        return;
    }

    add(shaderID, vertexCode, GL_VERTEX_SHADER);
    add(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shaderID);
    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }


    uModel = glGetUniformLocation(shaderID, "model");
    uProjection = glGetUniformLocation(shaderID, "projection");
    uView = glGetUniformLocation(shaderID, "view");
    uDirectionalLight.uColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
    uDirectionalLight.uAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
    uDirectionalLight.uDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uDirectionalLight.uDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
    uSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uShininess = glGetUniformLocation(shaderID, "material.shininess");
    uCameraPosition = glGetUniformLocation(shaderID, "cameraPosition");

    uPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char locBuffer[100] = {'\0'};

        snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].base.color", i);
        uPointLight[i].uColor = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].base.ambientIntensity", i);
        uPointLight[i].uAmbientIntensity = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].base.diffuseIntensity", i);
        uPointLight[i].uDiffuseIntensity = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].position", i);
        uPointLight[i].uPosition = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].constant", i);
        uPointLight[i].uConstant = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].linear", i);
        uPointLight[i].uLinear = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].exponent", i);
        uPointLight[i].uExponent = glGetUniformLocation(shaderID, locBuffer);
    }
}

void Shader::add(GLuint prog, const char* code, GLenum shaderType)
{
    GLuint shad = glCreateShader(shaderType);

    const GLchar* shadCode[1];
    shadCode[0] = code;

    GLint codeLen[1];
    codeLen[0] = strlen(code);

    glShaderSource(shad, 1, shadCode, codeLen);
    glCompileShader(shad);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    
    glGetShaderiv(shad, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(shad, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(prog, shad);
}