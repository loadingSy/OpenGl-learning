#include "Object.hpp"

#include <iterator>
#include <map>
#include <string>

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

void Object::changeRotation(const float yawRotation, const float pitchRotation)
{
    rotation.x += yawRotation;
    rotation.y += pitchRotation;
}

void Object::changeRotation(const vector2 rotation)
{
    this->rotation += rotation;
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
void Object::setUniform(const char *name, float value, bool useShader)
{
    if(useShader)
        mesh.getShader()->use();

    #ifdef DEBUG_UNIFORM_LOGS
    std::cout << "\nfloat, genius right?..  " << value << '\n';
    #endif

    glUniform1f(getUniformLocation(name), value);
}
template<>
void Object::setUniform(const char *name, int value, bool useShader)
{
    if(useShader)
        mesh.getShader()->use();

    #ifdef DEBUG_UNIFORM_LOGS
    std::cout << "\nint, genius right?..  " << value << '\n';
    #endif

    glUniform1i(getUniformLocation(name), value);
}
template<>
void Object::setUniform(const char *name, glm::mat4 value, bool useShader)
{
    if(useShader)
        mesh.getShader()->use();

    #ifdef DEBUG_UNIFORM_LOGS    
    std::cout << "\nmatrix, genius right?..  \n";
    #endif

    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}


glm::mat4 Object::getModelMatrix()
{
    glm::mat4 matrix(0.0f);
    matrix[3] = glm::vec4(position.x, position.y, position.z, 1);
    return matrix;
}

void Object::render( glm::mat4 view,  glm::mat4 projection)
{
    view = glm::mat4(view);
    projection = glm::mat4(projection);
    setUniform("view", view, true);
    setUniform("projection", projection);
    glDrawElements(GL_TRIANGLES, mesh.getIsize()/sizeof(GLuint), GL_UNSIGNED_INT, 0);
    
}