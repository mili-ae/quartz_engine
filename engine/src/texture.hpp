#pragma once
#include "../third_party/glew/glew.h"
#include "../third_party/stb_image.h"

class Texture {
public:
    Texture();
    Texture(char* path);
    ~Texture();

    void load();
    void use();
    void clear();

private:
    GLuint textureID;
    int width, height, bitDepth;
    char* filePath;
};
