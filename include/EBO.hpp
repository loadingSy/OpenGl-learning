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
        const void bind();
        const void unBind();
        const void destroy() {glDeleteBuffers(1, &id); id = 0;}

        //the move operator
        EBO &operator=(EBO &&other)
        {
            if (this == &other) return *this;
            this->id = other.id;
            this->iSize = other.iSize;
            other.id = 0;
            other.iSize = 0;
            return *this;
        }

        //the copy
        EBO(const EBO&) = delete;

        //move constructor (im still new to this but still learning new cpp concepts every day :)
        EBO(EBO&& other) noexcept : id(other.id), iSize(other.iSize) 
        {
            if(id != 0)
                destroy();
            other.id = 0;
            other.iSize = 0;
        }
        EBO& operator=(const EBO&) = delete;

        GLsizeiptr getIsize() {return iSize;}

    private:
        GLsizeiptr iSize = 0;

};