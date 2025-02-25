#include "engine.hpp"

Window window = Window();

EXPORT_FN void init()
{
    if (!window.init())
    {
        printf("Failed to init window");
    }
}

// void DirectionalShadowMapPass(DirectionalLight* light)
// {

//     glViewport(0, 0, light->shadowMap->shadowWidth, light->shadowMap->shadowHeight);
//     directionalShadowShader.use();
//     light->shadowMap->write();
//     glClear(GL_DEPTH_BUFFER_BIT);

//     uniformModel = directionalShadowShader.uModel;
//     directionalShadowShader.setDirectionalLightTransform(&light->calculateLightTransform());

//     directionalShadowShader.validate();
//     for(int i = 0; i < gameobjects.size(); i++)
//     {
//         gameobjects[i]->update();
//         gameobjects[i]->render();
//     }
//     glBindFramebuffer(GL_FRAMEBUFFER, 0);
// }

// void OmniShadowMapPass(PointLight* light)
// {

//     glViewport(0, 0, light->shadowMap->shadowWidth, light->shadowMap->shadowHeight);
//     omniShadowShader.use();
//     uniformModel = omniShadowShader.uModel;
//     uniformOmniLightPos = omniShadowShader.uOmniLightPos;
//     uniformFarPlane = omniShadowShader.uFarPlane;

//     light->shadowMap->write();
//     glClear(GL_DEPTH_BUFFER_BIT);

//     glUniform3f(uniformOmniLightPos, light->position.x, light->position.y, light->position.z);
//     glUniform1f(uniformFarPlane, light->farPlane);
//     omniShadowShader.setLightMatrices(light->calculateLightTransform());

//     omniShadowShader.validate();

//     for(int i = 0; i < gameobjects.size(); i++)
//     {
//         gameobjects[i]->update();
//         gameobjects[i]->render();
//     }
//     glBindFramebuffer(GL_FRAMEBUFFER, 0);
// }

// void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
// {
//     glViewport(0, 0, 1366, 768);

//     glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//     // call skybox before everything else
//     skybox.draw(viewMatrix, projectionMatrix);


//     shaders[0].use();
//     uniformModel = shaders[0].uModel;
//     uniformProjection = shaders[0].uProjection;
//     uniformView = shaders[0].uView;

//     uniformCameraPosition = shaders[0].uCameraPosition;
//     uniformSpecularIntensity = shaders[0].uSpecularIntensity;
//     uniformShininess = shaders[0].uShininess;

    

//     glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
//     glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
//     glUniform3f(uniformCameraPosition, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

//     shaders[0].setDirectionalLight(&mainLight);
//     shaders[0].setPointLights(pointLights, pointLightCount, 3, 0);
//     shaders[0].setSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
//     shaders[0].setDirectionalLightTransform(&mainLight.calculateLightTransform());
    
//     mainLight.shadowMap->read(GL_TEXTURE2);
//     shaders[0].setTexture(1);
//     shaders[0].setDirectionalShadowMap(2);
    
//     glm::vec3 lowerLight = camera.getPosition();
//     lowerLight.y -= 0.3f;
//     spotLights[0].SetFlash(lowerLight, camera.getDirection());

//     shaders[0].validate();
//     for(int i = 0; i < gameobjects.size(); i++)
//     {
//         gameobjects[i]->update();
//         gameobjects[i]->render();
//     }
// }

// void render(void (*func)() = nullptr)
// {
//     DirectionalShadowMapPass(&mainLight);

//     for (size_t i = 0; i < pointLightCount; i++)
//     {
//         OmniShadowMapPass(&pointLights[i]);
//     }

//     for (size_t i = 0; i < spotLightCount; i++)
//     {
//         OmniShadowMapPass(&spotLights[i]);
//     }

//     RenderPass(projection, camera.calculateViewMatrix());

//     if (window.getKeys()[GLFW_KEY_L])
//     {
//         spotLights[0].toggle();
//         window.getKeys()[GLFW_KEY_L] = false;
//     }
// }

EXPORT_FN void update(void (*func)() = nullptr)
{
    Renderer3D renderer3d = Renderer3D(&window, &camera, &skybox, projection);
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.2f);

    while (!window.shouldClose())
    {
        // Calculate deltaTime
        GLfloat current = glfwGetTime();
        deltaTime = current - lastTime;
        lastTime = current;
        float fps = 1.0f / deltaTime;
        // printf("FPS: %.4f\n", fps);

        glfwPollEvents();

        camera.keyControl(window.getKeys(), deltaTime);
        camera.mouseControl(window.getXChange(), window.getYChange());

        // glClearColor(128.0f / 255.0f, 218.0f / 255.0f, 243.0f / 255.0f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // func();
        // render(func);
        renderer3d.render(gameobjects);

        glUseProgram(0);

        window.swapBuffers();
    }
}

