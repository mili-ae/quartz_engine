#pragma once

#include "../third_party/glew/glew.h"
#include "../third_party/glm/glm.hpp"
#include "../third_party/glm/gtc/matrix_transform.hpp"
#include "../third_party/glfw/glfw3.h"

class Camera {
public:
    Camera();
    // Position, Up, YAW, Pitch
    Camera(glm::vec3 pos, glm::vec3 u, GLfloat ya, GLfloat p, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
    ~Camera();
    void keyControl(bool* keys, GLfloat deltaTime);
    void mouseControl(GLfloat xChange, GLfloat yChange);

    glm::vec3 getPosition() { return position; }
    glm::mat4 calculateViewMatrix();

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp; // World's up (sky)

    GLfloat yaw;
    GLfloat pitch;

    GLfloat movementSpeed; // Keyboard input speed
    GLfloat turnSpeed; // How fast it moves in reaction to the mouse

    void update(); // Updates the values every time something changes
};
