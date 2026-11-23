#pragma once
#include "glad/glad.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Vector.hpp"

class Camera
{
    public:
        glm::vec3 position;
        glm::vec3 up;
        glm::vec3 forward;
        glm::vec3 right;

        float yaw = -90, pitch = 0;

        float speed = 2;
        vector2 sensitivity = vector2(0.6f, 0.4f);

        Camera() : position(glm::vec3(0)), forward(0,0,-1), right(1,0,0), up(0,1,0) {}
        Camera(const vector2 pSensitivity) : sensitivity(pSensitivity){};
        Camera(const glm::vec3 pPos) : position(pPos), forward(0,0,-1), right(1,0,0), up(0,1,0) {}
        Camera(const glm::vec3 pPos, const glm::vec3 pUp, const glm::vec3 pForward) : position(pPos), up(pUp), forward(pForward), right(glm::cross(pUp, pForward)) {}
        Camera(const glm::vec3 pPos, const glm::vec3 pUp, const glm::vec3 pForward, const glm::vec3 pRight) : position(pPos), up(pUp), forward(pForward), right(pRight) {}

        //Returns the Camera Position, Rotation Matrix
        glm::mat4 const getMatrix();

        /*Multiplies with Speed, Great for free Movement*/
        void move(glm::vec3 movementVector);
        /*Multiplies with Speed, Great for free Movement*/
        void move(vector3 movementVector);

        /*Pure Transformation with no Speed, Great for setting Positions*/
        void changePos(glm::vec3 movementVector);
        /*Pure Transformation with no Speed, Great for setting Positions*/
        void changePos(vector3 movementVector);

        /*Rotation Multiplied With sensitivity, N angles yaw, pitch*/
        void rotate(float yawRotation, float pitchRotation);
        /*Rotation Multiplied With sensitivity, N angles yaw, pitch*/
        void rotate(vector2 rotation);

        /*Pure rotation with N angles yaw, pitch*/
        void changeRotation(float yawRotation, float pitchRotation);
        /*Pure rotation with N angles yaw, pitch*/
        void changeRotation(vector2 rotation);

        //Updates Up, Forward, Right vectors
        void updateCameraVectors();
};