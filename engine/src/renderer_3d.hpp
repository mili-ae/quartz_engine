#pragma once

#include "shader.hpp"
#include "window.hpp"
#include "camera/camera.hpp"
#include "lighting/light.hpp"
#include "skybox.hpp"
#include "game_object.hpp"
#include <vector>
// #include "lighting/directional_light.cpp"
// #include "lighting/omni_shadow_map.cpp"

class Renderer3D
{
public:
    Renderer3D();
    Renderer3D(Window *win, Camera *cam, Skybox *sky, glm::mat4 proj);
    ~Renderer3D();
    
    void DirectionalShadowMapPass(DirectionalLight* light, std::vector<GameObject*> gameobjects);
    void OmniShadowMapPass(PointLight* light, std::vector<GameObject*> gameobjects);
    void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, std::vector<GameObject*> gameobjects);
    void render(std::vector<GameObject*> gameobjects);

    GLuint uniformProjection, uniformModel, uniformView, uniformCameraPosition,
       uniformSpecularIntensity, uniformShininess, uniformOmniLightPos, uniformFarPlane;

private:
    std::vector<Shader> shaders;
    Shader directionalShadowShader;
    Shader omniShadowShader;

    DirectionalLight mainLight;
    PointLight pointLights[MAX_POINT_LIGHTS];
    unsigned int pointLightCount = 0;
    SpotLight spotLights[MAX_SPOT_LIGHTS];
    unsigned int spotLightCount = 0;

    Skybox *skybox;
    Camera *camera;
    glm::mat4 projection;

    Window *window;
};