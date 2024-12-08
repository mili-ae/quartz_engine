#include "shader.hpp"

Shader::Shader()
{
    shaderID = 0;
    uModel = 0;
    uView = 0;
    uProjection = 0;
    uDirectionalLight.uColor = 0;
    uDirectionalLight.uAmbientIntensity = 0;
    uDirectionalLight.uDirection = 0;
    uDirectionalLight.uDiffuseIntensity = 0;
    uCameraPosition = 0;
    uShininess = 0;
    uSpecularIntensity = 0;

    pointLightCount = 0;
    spotLightCount = 0;
}
Shader::~Shader()
{
}

void Shader::createFromString(const char* vertexCode, const char* fragmentCode)
{
    compile(vertexCode, fragmentCode);
}

void Shader::createFromFile(const char* vertexPath, const char* fragmentPath)
{
    compile(readFile(vertexPath).c_str(), readFile(fragmentPath).c_str());
}

void Shader::createFromFile(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
    compile(readFile(vertexPath).c_str(),readFile(geometryPath).c_str(), readFile(fragmentPath).c_str());
}

void Shader::setDirectionalLight(DirectionalLight *dLight)
{
    dLight->use(uDirectionalLight.uAmbientIntensity, uDirectionalLight.uColor, uDirectionalLight.uDiffuseIntensity, uDirectionalLight.uDirection);
}

void Shader::setPointLights(PointLight *pLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
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
        pLight[i].shadowMap->read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(uOmniShadowMap[i + offset].shadowMap, textureUnit + i);
        glUniform1f(uOmniShadowMap[i + offset].farPlane, pLight[i].farPlane);
    }
}

void Shader::setSpotLights(SpotLight *sLight, unsigned int lightCount, unsigned int textureUnit, unsigned int offset)
{
    if (lightCount > MAX_SPOT_LIGHTS)
    {
        lightCount = MAX_SPOT_LIGHTS;
    }

    glUniform1i(uSpotLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++)
    {
        sLight[i].use(uSpotLight[i].uAmbientIntensity, uSpotLight[i].uColor, 
                        uSpotLight[i].uDiffuseIntensity, uSpotLight[i].uPosition, uSpotLight[i].uDirection,
                        uSpotLight[i].uConstant, uSpotLight[i].uLinear, uSpotLight[i].uExponent, uSpotLight[i].uEdge);
        sLight[i].shadowMap->read(GL_TEXTURE0 + textureUnit + i);
        glUniform1i(uOmniShadowMap[i + offset].shadowMap, textureUnit + i);
        glUniform1f(uOmniShadowMap[i + offset].farPlane, sLight[i].farPlane);
    }
}

void Shader::setTexture(GLuint textureUnit)
{
    glUniform1i(uTexture, textureUnit);
}

void Shader::setDirectionalShadowMap(GLuint textureUnit)
{
    glUniform1i(uDirectionalShadowMap, textureUnit);
}

void Shader::setDirectionalLightTransform(glm::mat4* lTransform)
{
    glUniformMatrix4fv(uDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lTransform));
}

void Shader::setLightMatrices(std::vector<glm::mat4> lightMatrices)
{
    for (size_t i = 0; i < 6; i++)
    {
        glUniformMatrix4fv(uLightMatrices[i], 1, GL_FALSE, glm::value_ptr(lightMatrices[i]));
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

    compileProgram();
}

void Shader::compile(const char* vertexCode, const char* geometryCode, const char* fragmentCode)
{
    shaderID = glCreateProgram();

    if (!shaderID)
    {
        printf("Error creating shader program\n");
        return;
    }

    add(shaderID, vertexCode, GL_VERTEX_SHADER);
    add(shaderID, geometryCode, GL_GEOMETRY_SHADER);
    add(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    compileProgram();
}

void Shader::validate()
{
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }
}

void Shader::compileProgram()
{
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

        snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].base.color", (int)i);
        uPointLight[i].uColor = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].base.ambientIntensity", (int)i);
        uPointLight[i].uAmbientIntensity = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].base.diffuseIntensity", (int)i);
        uPointLight[i].uDiffuseIntensity = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].position", (int)i);
        uPointLight[i].uPosition = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].constant", (int)i);
        uPointLight[i].uConstant = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].linear", (int)i);
        uPointLight[i].uLinear = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "pointLights[%d].exponent", (int)i);
        uPointLight[i].uExponent = glGetUniformLocation(shaderID, locBuffer);
    }

    uSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");
    for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        char locBuffer[100] = {'\0'};

        snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].base.base.color", (int)i);
        uSpotLight[i].uColor = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].base.base.ambientIntensity", (int)i);
        uSpotLight[i].uAmbientIntensity = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].base.base.diffuseIntensity", (int)i);
        uSpotLight[i].uDiffuseIntensity = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].base.position", (int)i);
        uSpotLight[i].uPosition = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].base.constant", (int)i);
        uSpotLight[i].uConstant = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].base.linear", (int)i);
        uSpotLight[i].uLinear = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].base.exponent", (int)i);
        uSpotLight[i].uExponent = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].direction", (int)i);
        uSpotLight[i].uDirection = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "spotLights[%d].edge", (int)i);
        uSpotLight[i].uEdge = glGetUniformLocation(shaderID, locBuffer);
    }

    for (size_t i = 0; i < MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS; i++)
    {
        char locBuffer[100] = {'\0'};

        snprintf(locBuffer, sizeof(locBuffer), "omniShadowMaps[%d].shadowMap", (int)i);
        uOmniShadowMap[i].shadowMap = glGetUniformLocation(shaderID, locBuffer);

        snprintf(locBuffer, sizeof(locBuffer), "omniShadowMaps[%d].farPlane", (int)i);
        uOmniShadowMap[i].farPlane = glGetUniformLocation(shaderID, locBuffer);
    }

    uDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightSpaceTransform");
    uTexture = glGetUniformLocation(shaderID, "theTexture");
    uDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");

    uOmniLightPos = glGetUniformLocation(shaderID, "lightPos");
    uFarPlane = glGetUniformLocation(shaderID, "farPlane");

    for (size_t i = 0; i < 6; i++)
    {
        char locBuffer[100] = {'\0'};

        snprintf(locBuffer, sizeof(locBuffer), "lightMatrices[%d]", (int)i);
        uLightMatrices[i] = glGetUniformLocation(shaderID, locBuffer);
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