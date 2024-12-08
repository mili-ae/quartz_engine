#pragma once

#include <vector>
#include <string>

#include "../third_party/glew/glew.h"
#include "../third_party/glm/glm.hpp"
#include "../third_party/glm/gtc/matrix_transform.hpp"
#include "../third_party/glm/gtc/type_ptr.hpp"

#include "common.hpp"
#include "mesh.hpp"
#include "shader.hpp"

class Skybox {
public:
    Skybox();
    Skybox(std::vector<std::string> faceLocations);
    ~Skybox();

    void draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

private:
    Mesh *mesh;
    Shader *shader;

    GLuint textureID;
    GLuint uProjection, uView;
};
