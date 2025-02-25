#include "camera.hpp"

Camera::Camera()
{
    turnSpeed = 0;
}

Camera::Camera(glm::vec3 pos, glm::vec3 u, GLfloat ya, GLfloat p, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    position = pos;
    worldUp = u;
    yaw = ya;
    pitch = p;
    front = glm::vec3(0.0f, 0.0f, -1.0f);

    movementSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;

    update();
}

Camera::~Camera()
{
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
    GLfloat velocity = movementSpeed * deltaTime;
    if (keys[GLFW_KEY_W])
    {
        position += front * velocity ;
    }

    if (keys[GLFW_KEY_S])
    {
        position -= front * velocity;
    }

    if (keys[GLFW_KEY_A])
    {
        position -= right * velocity;
    }

    if (keys[GLFW_KEY_D])
    {
        position += right * velocity;
    }

    if (keys[GLFW_KEY_E])
    {
        position += up * velocity;
    }

    if (keys[GLFW_KEY_Q])
    {
        position -= up * velocity;
    }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
    xChange *= turnSpeed;
    yChange *= turnSpeed;

    yaw += xChange;
    pitch += yChange;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }

    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    update();
}

glm::mat4 Camera::calculateViewMatrix()
{
    return glm::lookAt(position, position + front, up);
}

void Camera::update()
{
    // Calculate front of the camera
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);

    // Right
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}
