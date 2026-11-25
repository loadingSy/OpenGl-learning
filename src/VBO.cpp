#include "glad/glad.h"
#include "../include/VBO.hpp"


VBO::VBO(const GLfloat *vertices, GLsizeiptr size)
{
    glGenBuffers(1, &id);
    setData(vertices, size);
}
VBO::VBO()
{
    glGenBuffers(1, &id);
}
void VBO::setData(const GLfloat *vertices, GLsizeiptr size)
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    vSize = size;
    unBind();
}

VBO::~VBO() {
    glDeleteBuffers(1, &id);
}

const void VBO::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

const void VBO::unBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}