#pragma once

#include "shadow_map.hpp"

class OmniShadowMap : public ShadowMap {
public:
    OmniShadowMap();
    ~OmniShadowMap();

    bool init(unsigned int width, unsigned int height);
    void write();
    void read(GLenum textureUnit);

private:

};