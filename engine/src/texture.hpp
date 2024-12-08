#pragma once

#include "../third_party/glew/glew.h"
#include "common.hpp"

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
