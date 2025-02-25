#include "renderer_3d.hpp"

Renderer3D::Renderer3D()
{

}

Renderer3D::Renderer3D(Window *win, Camera *cam, Skybox *sky, glm::mat4 proj)
{
    window = win;
    camera = cam;
    skybox = sky;
    projection = proj;

    Shader *shader1 = new Shader();
    shader1->createFromFile("assets/shaders/shader.vert", "assets/shaders/shader.frag");
    shaders.push_back(*shader1);

    directionalShadowShader = Shader();
    directionalShadowShader.createFromFile("assets/shaders/directional_shadow_map.vert", "assets/shaders/directional_shadow_map.frag");
    omniShadowShader = Shader();
    omniShadowShader.createFromFile("assets/shaders/omni_shadow_map.vert", "assets/shaders/omni_shadow_map.geom", "assets/shaders/omni_shadow_map.frag");


    // TODO:: Set lights as entities to be made by the user
    mainLight = DirectionalLight(2048, 2048, glm::vec3(1.0f, 0.53f, 0.3f), 0.1f, 0.8f, glm::vec3(-10.0f, -12.0f, 18.5f));

    pointLights[0] = PointLight(1024, 1024, 0.01f, 100.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.0f, glm::vec3(1.0f, 2.0f, 0.0f), 0.3f, 0.2f, 0.1f);
    pointLightCount++;
    // pointLights[1] = PointLight(1024, 1024, 0.01f, 100.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.0f, glm::vec3(-4.0f, 2.0f, 0.0f), 0.3f, 0.2f, 0.1f);
    // pointLightCount++;

    // spotLights[0] = SpotLight(1024, 1024, 0.01f, 100.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 2.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f, 0.0f, 10.0f);
    // spotLightCount++;
    // spotLights[1] = SpotLight(1024, 1024, 0.01f, 100.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.0f, glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(-100.0f, -1.0f, 0.0f), 1.0f, 0.0f, 0.0f, 20.0f);
    // spotLightCount++;
}

Renderer3D::~Renderer3D()
{
}

void Renderer3D::DirectionalShadowMapPass(DirectionalLight* light, std::vector<GameObject*> gameobjects)
{

    glViewport(0, 0, light->shadowMap->shadowWidth, light->shadowMap->shadowHeight);
    directionalShadowShader.use();
    light->shadowMap->write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = directionalShadowShader.uModel;
    directionalShadowShader.setDirectionalLightTransform(&light->calculateLightTransform());

    directionalShadowShader.validate();
    for(int i = 0; i < gameobjects.size(); i++)
    {
        gameobjects[i]->update(uniformModel, uniformSpecularIntensity, uniformShininess);
        gameobjects[i]->render();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer3D::OmniShadowMapPass(PointLight* light, std::vector<GameObject*> gameobjects)
{

    glViewport(0, 0, light->shadowMap->shadowWidth, light->shadowMap->shadowHeight);
    omniShadowShader.use();
    uniformModel = omniShadowShader.uModel;
    uniformOmniLightPos = omniShadowShader.uOmniLightPos;
    uniformFarPlane = omniShadowShader.uFarPlane;

    light->shadowMap->write();
    glClear(GL_DEPTH_BUFFER_BIT);

    glUniform3f(uniformOmniLightPos, light->position.x, light->position.y, light->position.z);
    glUniform1f(uniformFarPlane, light->farPlane);
    omniShadowShader.setLightMatrices(light->calculateLightTransform());

    omniShadowShader.validate();

    for(int i = 0; i < gameobjects.size(); i++)
    {
        gameobjects[i]->update(uniformModel, uniformSpecularIntensity, uniformShininess);
        gameobjects[i]->render();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer3D::RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, std::vector<GameObject*> gameobjects)
{
    glViewport(0, 0, 1366, 768);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // call skybox before everything else
    skybox->draw(viewMatrix, projectionMatrix);


    shaders[0].use();
    uniformModel = shaders[0].uModel;
    uniformProjection = shaders[0].uProjection;
    uniformView = shaders[0].uView;

    uniformCameraPosition = shaders[0].uCameraPosition;
    uniformSpecularIntensity = shaders[0].uSpecularIntensity;
    uniformShininess = shaders[0].uShininess;

    

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniform3f(uniformCameraPosition, camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

    shaders[0].setDirectionalLight(&mainLight);
    shaders[0].setPointLights(pointLights, pointLightCount, 3, 0);
    shaders[0].setSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
    shaders[0].setDirectionalLightTransform(&mainLight.calculateLightTransform());
    
    mainLight.shadowMap->read(GL_TEXTURE2);
    shaders[0].setTexture(1);
    shaders[0].setDirectionalShadowMap(2);
    
    glm::vec3 lowerLight = camera->getPosition();
    lowerLight.y -= 0.3f;
    spotLights[0].SetFlash(lowerLight, camera->getDirection());

    shaders[0].validate();
    for(int i = 0; i < gameobjects.size(); i++)
    {
        gameobjects[i]->update(uniformModel, uniformSpecularIntensity, uniformShininess);
        gameobjects[i]->render();
    }
}

void Renderer3D::render(std::vector<GameObject*> gameobjects)
{
    DirectionalShadowMapPass(&mainLight, gameobjects);

    for (size_t i = 0; i < pointLightCount; i++)
    {
        OmniShadowMapPass(&pointLights[i], gameobjects);
    }

    for (size_t i = 0; i < spotLightCount; i++)
    {
        OmniShadowMapPass(&spotLights[i], gameobjects);
    }

    RenderPass(projection, camera->calculateViewMatrix(), gameobjects);

    // if (window.getKeys()[GLFW_KEY_L])
    // {
    //     spotLights[0].toggle();
    //     window.getKeys()[GLFW_KEY_L] = false;
    // }
}