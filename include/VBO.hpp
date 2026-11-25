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

        const void bind();
        const void unBind();
        const void destroy() {glDeleteBuffers(1, &id); id = 0;}

        VBO &operator=(VBO&& other)
        {
            if(this == &other) return *this;

            if (id != 0)
                destroy();
            this->id = other.id;
            this->vSize = other.vSize;
            other.id = 0;
            other.vSize = 0;
    

            return *this;
        }

        VBO(const VBO&) = delete;

        VBO(VBO &&other) noexcept : id(other.id), vSize(other.vSize)
        {
            if(id != 0)
                destroy();
            other.id = 0;
            other.vSize = 0;
        }

        VBO& operator=(const VBO&) = delete;

        GLsizeiptr getVsize() {return vSize;}
    private:
        GLsizeiptr vSize = 0;


};