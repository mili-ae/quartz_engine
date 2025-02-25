#pragma once

#include "../third_party/stb_image.h"

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformCameraPosition = 0,
       uniformSpecularIntensity = 0, uniformShininess = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;