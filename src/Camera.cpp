#include "Camera.hpp"
#include "Vector.hpp"
#include <algorithm>
#include <cmath>
#include <math.h>


glm::mat4 const Camera::getMatrix()
{
    return glm::lookAt(position, position + forward, up);
};

void Camera::move(glm::vec3 movementVector)
{
    position += (movementVector.x * glm::normalize(right) + movementVector.y * glm::vec3(0,1,0) + movementVector.z * glm::normalize(forward)) * speed;
}

void Camera::move(vector3 movementVector)
{
    move(glm::vec3(movementVector.x, movementVector.y, movementVector.z));
}

void Camera::changePos(glm::vec3 movementVector)
{
    position += movementVector.x * glm::normalize(right) + movementVector.y * glm::normalize(up) + movementVector.z * glm::normalize(forward);
}

void Camera::changePos(vector3 movementVector)
{
    changePos(glm::vec3(movementVector.x, movementVector.y, movementVector.z));
}

void Camera::updateCameraVectors()
{
    //the axis which is degree number 0 or the start
    //is given cos, the one that is 90 degrees to it is given sin
    //the opposite direction is -cos and -sin

    //for pitch the axes where you start for examples here the ground
    //axes are x and z so they get multiplied by cos
    //the up axis gets -sin


    pitch = std::clamp(pitch, -89.0f, 89.0f);
    float yawRadian = glm::radians(yaw);
    float pitchRadian = glm::radians(pitch);

    forward.x = sin(yawRadian) * cos(pitchRadian);
    forward.z = -cos(yawRadian) * cos(pitchRadian);
    forward.y = sin(pitchRadian);

    forward = glm::normalize(forward);


    right.x = cos(yawRadian);
    right.z = sin(yawRadian); 
    right.y = 0.0f;

    right = glm::normalize(right);

    up = glm::normalize(glm::cross(right, forward));

}

void Camera::rotate(float yawRotation, float pitchRotation)
{
    yaw += yawRotation * sensitivity.x;
    pitch += pitchRotation * sensitivity.y;
    updateCameraVectors();
}

void Camera::rotate(vector2 rotation)
{
    rotate(rotation.x, rotation.y);
}

void Camera::changeRotation(float yawRotation, float pitchRotation)
{
    yaw += yawRotation;
    pitch += pitchRotation;
    updateCameraVectors();
}

void Camera::changeRotation(vector2 rotation)
{
    changeRotation(rotation.x, rotation.y);
}