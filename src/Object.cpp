#include "Object.hpp"

#include <iterator>
#include <map>
#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

// #define DEBUG_UNIFORM_LOGS

#ifdef DEBUG_UNIFORM_LOGS 
#include <iostream>
#endif

void Object::changePos(const glm::vec3 movementVector)
{
    position += movementVector;
}
void Object::changePos(const vector3 movementVector)
{
    position += movementVector;
}

void Object::changeRotation(const float yawRotation, const float pitchRotation, const float rollRotation)
{
    rotation.x += yawRotation;
    rotation.y += pitchRotation;
    rotation.z += rollRotation;
}

void Object::changeRotation(const vector3 rotation)
{
    this->rotation += rotation;
}



GLuint Object::getUniformLocation(const char *name,const bool cache)
{
    bool found = uniformsLookup.find(name) != std::end(uniformsLookup);
    
    if(found)
        return uniformsLookup[name];

    GLuint uId = mesh->getShader()->getUniformLocation(name);

    if(cache)
        uniformsLookup[name] = uId;

    return uId;
}


template<>
void Object::setUniform(const char *name, float value, bool useShader)
{
    if(useShader)
        mesh->getShader()->use();

    #ifdef DEBUG_UNIFORM_LOGS
    std::cout << "\nfloat, genius right?..  " << value << '\n';
    #endif

    glUniform1f(getUniformLocation(name), value);
}
template<>
void Object::setUniform(const char *name, int value, bool useShader)
{
    if(useShader)
        mesh->getShader()->use();

    #ifdef DEBUG_UNIFORM_LOGS
    std::cout << "\nint, genius right?..  " << value << '\n';
    #endif

    glUniform1i(getUniformLocation(name), value);
}
template<>
void Object::setUniform(const char *name, glm::mat4 value, bool useShader)
{
    if(useShader)
        mesh->getShader()->use();

    #ifdef DEBUG_UNIFORM_LOGS    
    std::cout << "\nmatrix, genius right?..  \n";
    #endif

    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

template<>
void Object::setUniform(const char *name, glm::vec3 value, bool useShader)
{
    if(useShader)
        mesh->getShader()->use();

    #ifdef DEBUG_UNIFORM_LOGS    
    std::cout << "\vec3, genius right?..  \n";
    #endif

    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}
template<>
void Object::setUniform(const char *name, bool value, bool useShader)
{
    if(useShader)
        mesh->getShader()->use();

    #ifdef DEBUG_UNIFORM_LOGS
    std::cout << "\nint, genius right?..  " << value << '\n';
    #endif

    glUniform1i(getUniformLocation(name), value);
}

glm::mat4 Object::getModelMatrix()
{
    glm::mat4 matrix = glm::translate(glm::mat4(1.0f), position);
    matrix *= glm::yawPitchRoll(glm::radians(rotation.y), glm::radians(rotation.x), glm::radians(rotation.z));;
    matrix *= glm::scale(glm::mat4(1), glm::vec3(scale));
    return matrix;
}

void Object::render( glm::mat4 view,  glm::mat4 projection)
{
    view = glm::mat4(view);
    projection = glm::mat4(projection);
    setUniform("view", view, true);
    setUniform("projection", projection);
    setUniform("model", getModelMatrix());
    glDrawElements(GL_TRIANGLES, mesh->getIsize()/sizeof(GLuint), GL_UNSIGNED_INT, 0);
    
}

