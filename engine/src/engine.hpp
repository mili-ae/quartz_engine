#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "quartz_lib.hpp"
#include "window.cpp"
#include "camera/camera.cpp"
#include "common.hpp"

#include "models/mesh.cpp"
#include "shader.cpp"
#include "texture.cpp"
#include "lighting/directional_light.cpp"
#include "lighting/point_light.cpp"
#include "lighting/spot_light.cpp"
#include "models/material.cpp"
#include "models/model.cpp"
#include "skybox.cpp"
#include "game_object.cpp"
#include "renderer_3d.cpp"

Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

const float toRadians = 3.14159265f / 180.0f;


glm::mat4 projection;
glm::mat4 model(1.0f);

// std::vector<Shader> shaders;
// Shader directionalShadowShader;
// Shader omniShadowShader;

std::vector<Mesh*> meshes;

// DirectionalLight mainLight;
// PointLight pointLights[MAX_POINT_LIGHTS];
// unsigned int pointLightCount = 0;
// SpotLight spotLights[MAX_SPOT_LIGHTS];
// unsigned int spotLightCount = 0;

Skybox skybox;

std::vector<GameObject*> gameobjects;