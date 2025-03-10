#include "texture.hpp"

Texture::Texture()
{
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    filePath = "";
}

Texture::Texture(const char* path)
{
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    filePath = path;

    // if (!load()) {
    //     printf("Failed to load texture: %s\n", path);
    // }
}

Texture::~Texture()
{
    clear();
}

bool Texture::load()
{
    unsigned char *texData = stbi_load(filePath, &width, &height, &bitDepth, STBI_rgb_alpha);
    if (!texData)
    {
        printf("Failed to find: %s\n", filePath);
        return false;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(texData);

    return true;
}

void Texture::use()
{
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::clear()
{
    glDeleteTextures(1, &textureID);
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    filePath = "";
}