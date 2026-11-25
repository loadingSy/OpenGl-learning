#pragma once
#include "glad/glad.h"
#include "VBO.hpp"


class VAO
{
    public:
        GLuint id;
        VAO();
        ~VAO();
        const void bind();
        const void unBind();
        const void destroy() {glDeleteVertexArrays(1, &id); id = 0;}

        void linkAttribute(VBO &vbo, GLuint layout, GLint numComponents, GLenum type, GLsizei stride, const void* offset);


        VAO &operator=(VAO&& other)
        {
            if(this == &other) return *this;

            if(id != 0)
                destroy();
            this->id = other.id;
            other.id = 0;

            return *this;
        }

        VAO(const VAO&) = delete;

        VAO(VAO &&other) noexcept : id(other.id)
        {
            other.id = 0;
        }

        VAO& operator=(const VAO&) = delete;

    private:
};
