#pragma once
#include "glad/glad.h"


class EBO
{
    public:
        GLuint id;
        EBO(const GLuint *indices, GLsizeiptr size);
        EBO();
        ~EBO();
        void setData(const GLuint *indices, GLsizeiptr size);
        void bind();
        void unBind();
    private:


};