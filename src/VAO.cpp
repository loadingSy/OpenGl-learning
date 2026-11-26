#include "../include/VAO.hpp"


VAO::VAO()
{
    glGenVertexArrays(1, &id);
}

VAO::~VAO()
{
    if(id != 0)
    glDeleteVertexArrays(1, &id);
}

void VAO::bind()
{
    glBindVertexArray(id);
}

void VAO::unBind()
{
    glBindVertexArray(0);
}

void VAO::linkAttribute(VBO &vbo, GLuint layout, GLint numComponents, GLenum type, GLsizei stride, const void* offset)
{
    bind();
    vbo.bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);    
    unBind();
    vbo.unBind();
}

