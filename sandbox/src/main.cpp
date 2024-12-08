#include <vector>
#include "engine.cpp"

const float toRadians = 3.14159265f / 180.0f;

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformCameraPosition = 0,
       uniformSpecularIntensity = 0, uniformShininess = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;
glm::mat4 projection;
glm::mat4 model(1.0f);

std::vector<Shader> shaders;
Shader directionalShadowShader;
Shader omniShadowShader;

Texture brickTexture;
Texture concreteTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

std::vector<Mesh*> meshes;
Model miku;
Model seahawk;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
unsigned int pointLightCount = 0;
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned int spotLightCount = 0;

GLfloat seahawkAngle = 0.0f;

void calculateAverageNormals(unsigned int *indices, unsigned int indiceCount, GLfloat *vertices,
                             unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;

        vertices[in0] += normal.x;
        vertices[in0 + 1] += normal.y;
        vertices[in0 + 2] += normal.z;

        vertices[in1] += normal.x;
        vertices[in1 + 1] += normal.y;
        vertices[in0 + 2] += normal.z;

        vertices[in2] += normal.x;
        vertices[in2 + 1] += normal.y;
        vertices[in2 + 2] += normal.z;
    }

    for (size_t i = 0; i < verticeCount / vLength; i++)
    {
        unsigned int nOffset = i * vLength + normalOffset;
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x;
        vertices[nOffset + 1] = vec.y;
        vertices[nOffset + 2] = vec.z;
    }
}

void CreateObjects()
{
    unsigned int indices[] =
    {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    GLfloat vertices[] =
    {
    //  x       y     z             u     v         nx    ny    nz
        -1.0f, -1.0f, -0.6f,         0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
        0.0f,  -1.0f, 1.0f,         0.5f, 0.0f,     0.0f, 0.0f, 0.0f,
        1.0f,  -1.0f, -0.6f,         1.0f, 0.0f,     0.0f, 0.0f, 0.0f,
        0.0f,   1.0f, 0.0f,         0.5f, 1.0f,     0.0f, 0.0f, 0.0f
    };

    unsigned int floorIndices[] =
    {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] =
    {
        -10.0f, 0.0f, -10.0f,       0.0f,   0.0f,       0.0f, -1.0f, 0.0f,
        10.0f,  0.0f, -10.0f,       10.0f,  0.0f,       0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f,  10.0f,       0.0f,  10.0f,       0.0f, -1.0f, 0.0f,
        10.0f,  0.0f,  10.0f,       10.0f, 10.0f,       0.0f, -1.0f, 0.0f
    };

    calculateAverageNormals(indices, 12, vertices, 32, 8, 5);

    Mesh *floor = new Mesh();
    floor->create(floorVertices, floorIndices, 32, 6);
    meshes.push_back(floor);
}

void CreateShaders()
{
    Shader *shader1 = new Shader();
    shader1->createFromFile("assets/shaders/shader.vert", "assets/shaders/shader.frag");
    shaders.push_back(*shader1);

    directionalShadowShader = Shader();
    directionalShadowShader.createFromFile("assets/shaders/directional_shadow_map.vert", "assets/shaders/directional_shadow_map.frag");
    omniShadowShader = Shader();
    omniShadowShader.createFromFile("assets/shaders/omni_shadow_map.vert", "assets/shaders/omni_shadow_map.geom", "assets/shaders/omni_shadow_map.frag");
}

void RenderScene()
{

    // Floor
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    concreteTexture.use();
    dullMaterial.use(uniformSpecularIntensity, uniformShininess);
    meshes[0]->render();

    // Imported models
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.06f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial.use(uniformSpecularIntensity, uniformShininess);
    miku.render();

    seahawkAngle += 15.0f * deltaTime;
    if (seahawkAngle > 360.0f) seahawkAngle = 0.1f;

    model = glm::mat4(1.0f);
    model = glm::rotate(model, -seahawkAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(4.0f, 1.0f, 0.0f));
    model = glm::rotate(model, 20.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial.use(uniformSpecularIntensity, uniformShininess);
    seahawk.render();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{

    glViewport(0, 0, light->shadowMap->shadowWidth, light->shadowMap->shadowHeight);
    directionalShadowShader.use();
    light->shadowMap->write();
    glClear(GL_DEPTH_BUFFER_BIT);

    uniformModel = directionalShadowShader.uModel;
    directionalShadowShader.setDirectionalLightTransform(&light->calculateLightTransform());

    directionalShadowShader.validate();
    RenderScene();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
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

    RenderScene();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
    shaders[0].use();
    uniformModel = shaders[0].uModel;
    uniformProjection = shaders[0].uProjection;
    uniformView = shaders[0].uView;

    uniformCameraPosition = shaders[0].uCameraPosition;
    uniformSpecularIntensity = shaders[0].uSpecularIntensity;
    uniformShininess = shaders[0].uShininess;

    glViewport(0, 0, 1366, 768);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniform3f(uniformCameraPosition, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

    shaders[0].setDirectionalLight(&mainLight);
    shaders[0].setPointLights(pointLights, pointLightCount, 3, 0);
    shaders[0].setSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
    shaders[0].setDirectionalLightTransform(&mainLight.calculateLightTransform());
    
    mainLight.shadowMap->read(GL_TEXTURE2);
    shaders[0].setTexture(1);
    shaders[0].setDirectionalShadowMap(2);
    
    glm::vec3 lowerLight = camera.getPosition();
    lowerLight.y -= 0.3f;
    spotLights[0].SetFlash(lowerLight, camera.getDirection());

    shaders[0].validate();
    RenderScene();
}

void render()
{
    DirectionalShadowMapPass(&mainLight);

    for (size_t i = 0; i < pointLightCount; i++)
    {
        OmniShadowMapPass(&pointLights[i]);
    }

    for (size_t i = 0; i < spotLightCount; i++)
    {
        OmniShadowMapPass(&spotLights[i]);
    }

    RenderPass(projection, camera.calculateViewMatrix());

    if (window.getKeys()[GLFW_KEY_L])
    {
        spotLights[0].toggle();
        window.getKeys()[GLFW_KEY_L] = false;
    }
}

int main(int argc, char *args[])
{
    init();
    CreateObjects();
    CreateShaders();
    
    projection = glm::perspective(glm::radians(60.0f), (GLfloat)window.bufferWidth / window.bufferHeight, 0.1f, 100.0f);

    brickTexture = Texture("assets/textures/brick.png");
    brickTexture.load();
    concreteTexture = Texture("assets/textures/cracked_concrete.jpg");
    concreteTexture.load();
    plainTexture = Texture("assets/textures/plain.png");
    plainTexture.load();

    shinyMaterial = Material(1.0f, 32);
    dullMaterial = Material(0.3f, 4);

    miku = Model();
    miku.load("assets/models/mecha_miku/scene.gltf");
    seahawk = Model();
    seahawk.load("assets/models/seahawk/Seahawk.obj");

    mainLight = DirectionalLight(2048, 2048, glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 0.3f, glm::vec3(0.0f, -15.0f, -10.0f));

    pointLights[0] = PointLight(1024, 1024, 0.01f, 100.0f, glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 0.3f, 0.2f, 0.1f);
    pointLightCount++;
    pointLights[1] = PointLight(1024, 1024, 0.01f, 100.0f, glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f, glm::vec3(-4.0f, 2.0f, 0.0f), 0.3f, 0.2f, 0.1f);
    pointLightCount++;

    spotLights[0] = SpotLight(1024, 1024, 0.01f, 100.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 2.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f, 0.0f, 10.0f);
    spotLightCount++;
    spotLights[1] = SpotLight(1024, 1024, 0.01f, 100.0f, glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.0f, glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(-100.0f, -1.0f, 0.0f), 1.0f, 0.0f, 0.0f, 20.0f);
    spotLightCount++;

    update(&render);

    return 0;
}