#include "Object.hpp"
#include <cstddef>
#include <iostream>
#include <iterator>
#include <map>
#include <string>


void Object::changePos(const glm::vec3 movementVector)
{
    position += movementVector;
}
void Object::changePos(const vector3 movementVector)
{
    position += movementVector;
}

void Object::changeRotation(const float yawRotation, const float pitchRotation)
{
    rotation.x += yawRotation;
    rotation.y += pitchRotation;
}

void Object::changeRotation(const vector2 rotation)
{
    this->rotation += rotation;
}

void Object::render()
{

    glDrawElements(GL_TRIANGLES, mesh.getIsize()/sizeof(GLuint), GL_UNSIGNED_INT, 0);
    
}

GLuint Object::getUniformLocation(const char *name,const bool cache)
{
    bool found = uniformsLookup.find(name) != std::end(uniformsLookup);
    
    if(found)
        return uniformsLookup[name];

    GLuint uId = mesh.getShader()->getUniformLocation(name);

    if(cache)
        uniformsLookup[name] = uId;

    return uId;
}


template<>
void Object::setUniform(const char *name, float value)
{

}
template<>
void Object::setUniform(const char *name, int value)
{
    std::cout << "int, genius right?";
}
template<>
void Object::setUniform(const char *name, glm::mat4 value)
{

}


