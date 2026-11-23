#include "Object.hpp"


void Object::changePos(glm::vec3 movementVector)
{
    position += movementVector;
}
void Object::changePos(vector3 movementVector)
{
    position += movementVector;
}

void Object::changeRotation(float yawRotation, float pitchRotation)
{
    rotation.x += yawRotation;
    rotation.y += pitchRotation;
}

void Object::changeRotation(vector2 rotation)
{
    this->rotation += rotation;
}

void Object::render()
{
    
}

