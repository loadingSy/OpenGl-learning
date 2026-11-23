#pragma once
#include "glad/glad.h"


class VBO
{
    public:
        GLuint id; 
        VBO(const GLfloat *vertices, GLsizeiptr size);
        VBO();
        void setData(const GLfloat *vertices, GLsizeiptr size);
        ~VBO();

        void bind();
        void unBind();
    private:



};