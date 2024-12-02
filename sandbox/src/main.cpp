#include <vector>

#include "engine.cpp"

#include "../../engine/third_party/glm/glm.hpp"
#include "../../engine/third_party/glm/gtc/matrix_transform.hpp"
#include "../../engine/third_party/glm/gtc/type_ptr.hpp"

std::vector<Mesh*> meshes;
std::vector<Shader> shaders;
const float toRadians = 3.14159265f / 180.0f;

static const char* vShader = "assets/shaders/shader.vert";
static const char* fShader = "assets/shaders/shader.frag";

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformCameraPosition = 0,
       uniformSpecularIntensity = 0, uniformShininess = 0;
glm::mat4 projection;

Texture brickTexture;
Texture concreteTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

Model miyako;
Model xwing;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
unsigned int pointLightCount = 0;
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned int spotLightCount = 0;

float curAngle = 0.0f;

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

    Mesh *obj1 = new Mesh();
    obj1->create(vertices, indices, 32, 12);
    meshes.push_back(obj1);

    Mesh *obj2 = new Mesh();
    obj2->create(vertices, indices, 32, 12);
    meshes.push_back(obj2);

    Mesh *floor = new Mesh();
    floor->create(floorVertices, floorIndices, 32, 6);
    meshes.push_back(floor);
}

void CreateShaders()
{
  Shader *shader1 = new Shader();
  shader1->createFromFile(vShader, fShader);
  shaders.push_back(*shader1);
}

void render()
{
    // Rotate
    curAngle += 7.0f * deltaTime;
    if (curAngle >= 360) curAngle -= 360;

    shaders[0].use();
    uniformModel = shaders[0].uModel;
    uniformProjection = shaders[0].uProjection;
    uniformView = shaders[0].uView;

    uniformCameraPosition = shaders[0].uCameraPosition;
    uniformSpecularIntensity = shaders[0].uSpecularIntensity;
    uniformShininess = shaders[0].uShininess;

    glm::vec3 lowerLight = camera.getPosition();
    lowerLight.y -= 0.3f;
    // spotLights[0].SetFlash(lowerLight, camera.getDirection());

    shaders[0].setDirectionalLight(&mainLight);
    shaders[0].setPointLights(pointLights, pointLightCount);
    shaders[0].setSpotLights(spotLights, spotLightCount);

    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
    // model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    // model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    
    // glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    // glUniform3f(uniformCameraPosition, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
    // brickTexture.use();
    // shinyMaterial.use(uniformSpecularIntensity, uniformShininess);
    // meshes[0]->render();

    // model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
    // glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    // concreteTexture.use();
    // dullMaterial.use(uniformSpecularIntensity, uniformShininess);
    // meshes[1]->render();

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    concreteTexture.use();
    dullMaterial.use(uniformSpecularIntensity, uniformShininess);
    meshes[2]->render();


    // Imported models
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
    // model = glm::rotate(model, -90.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.06f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    shinyMaterial.use(uniformSpecularIntensity, uniformShininess);
    miyako.render();

    
}

int main()
{
    init();
    CreateObjects();
    CreateShaders();
    
    projection = glm::perspective(45.0f, (GLfloat)window.bufferWidth / window.bufferHeight, 0.1f, 100.0f);

    brickTexture = Texture("assets/brick.png");
    brickTexture.loadAlpha();
    concreteTexture = Texture("assets/cracked_concrete.jpg");
    concreteTexture.load();
    plainTexture = Texture("assets/plain.png");
    plainTexture.loadAlpha();

    shinyMaterial = Material(1.0f, 32);
    dullMaterial = Material(0.3f, 4);

    miyako = Model();
    miyako.load("assets/models/mecha_miku/scene.gltf");

    mainLight = DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.3f, 0.6f, glm::vec3(0.0f, 0.0f, -1.0f));

    pointLights[0] = PointLight(glm::vec3(0.0f, 0.0f, 1.0f), 0.2f, 0.1f, glm::vec3(4.0f, 0.0f, 0.0f), 0.3f, 0.2f, 0.1f);
    // pointLightCount++;
    pointLights[1] = PointLight(glm::vec3(0.0f, 1.0f, 0.0f), 0.2f, 0.1f, glm::vec3(-4.0f, 2.0f, 0.0f), 0.3f, 0.2f, 0.1f);
    // pointLightCount++;

    spotLights[0] = SpotLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 2.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f, 0.0f, 10.0f);
    spotLightCount++;
    spotLights[1] = SpotLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 1.0f, glm::vec3(0.0f, -1.5f, 0.0f), glm::vec3(-100.0f, -1.0f, 0.0f), 1.0f, 0.0f, 0.0f, 20.0f);
    // spotLightCount++;

    update(&render);
}