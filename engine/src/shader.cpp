#include "shader.hpp"

Shader::Shader()
{
    shaderID = 0;
    uModel = 0;
    uProjection = 0;
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