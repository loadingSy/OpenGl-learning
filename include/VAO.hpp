#pragma once
#include "glad/glad.h"
#include "VBO.hpp"


class VAO
{
    public:
        GLuint id;
        VAO();
        ~VAO();
        void bind();
        void unBind();

        void linkAttribute(VBO &vbo, GLuint layout, GLint numComponents, GLenum type, GLsizei stride, const void* offset);

    private:
};
