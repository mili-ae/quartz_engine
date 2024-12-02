#pragma once
#include "../third_party/glew/glew.h"
#include "../third_party/stb_image.h"

class Texture {
public:
    Texture();
    Texture(const char* path);
    ~Texture();

    bool load();
    bool loadAlpha();
    void use();
    void clear();

private:
    GLuint textureID;
    int width, height, bitDepth;
    const char* filePath;
};
