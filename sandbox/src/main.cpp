#include "engine.cpp"
#include "mesh.cpp"
#include "shader.cpp"
#include <vector>

#include <../../engine/third_party/glm/glm.hpp>
#include <../../engine/third_party/glm/gtc/matrix_transform.hpp>
#include <../../engine/third_party/glm/gtc/type_ptr.hpp>

std::vector<Mesh*> meshes;
std::vector<Shader> shaders;
const float toRadians = 3.14159265f / 180.0f;

static const char* vShader = "shaders/shader.vert";
static const char* fShader = "shaders/shader.frag";

GLuint uniformProjection = 0, uniformModel = 0;
glm::mat4 projection;

float curAngle = 0.0f;

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
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    Mesh *obj1 = new Mesh();
    obj1->create(vertices, indices, 12, 12);
    meshes.push_back(obj1);
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
    curAngle += 0.6f;
    if (curAngle >= 360) curAngle -= 360;

    shaders[0].use();
    uniformModel = shaders[0].uModel;
    uniformProjection = shaders[0].uProjection;

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
    model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
    meshes[0]->render();
}

int main()
{
    init();
    CreateObjects();
    CreateShaders();

    projection = glm::perspective(45.0f, (GLfloat)window.bufferWidth / window.bufferHeight, 0.1f, 100.0f);
    update(&render);
}