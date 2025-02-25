#pragma once

#include <stdio.h>

#include "../../third_party/glew/glew.h"

class ShadowMap {
public:
    GLuint shadowWidth, shadowHeight;
    
    ShadowMap();
    ~ShadowMap();
    virtual bool init(GLuint width, GLuint height);
    virtual void write();
    virtual void read(GLenum textureUnit);

protected:
    GLuint FBO, shadowMap;
};
