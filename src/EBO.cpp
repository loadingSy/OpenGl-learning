#include "../include/EBO.hpp"
#include "glad/glad.h"


EBO::EBO(const GLuint *indices, GLsizeiptr size)
{
    glGenBuffers(1, &id);
    setData(indices, size);
}
EBO::EBO()
{
    glGenBuffers(1, &id);
}
void EBO::setData(const GLuint *indices, GLsizeiptr size)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
    iSize = size;
    unBind();
}

const void EBO::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

const void EBO::unBind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

EBO::~EBO()
{
    glDeleteBuffers(1, &id);
}