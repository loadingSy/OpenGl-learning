#pragma once
#include "glad/glad.h"
#include <string>


class Shader
{
    public:
        GLuint id = 0;
        void assignShader(std::string vertexShaderFile, std::string fragmentShaderFile);

        Shader(std::string vertexShaderFile, std::string fragmentShaderFile);
        Shader();
        ~Shader();
        const void use();
        const void unUse();
        void destroy();
        GLuint getUniformLocation(const char* name);

        Shader &operator=(Shader&& other)
        {
            if(this == &other) return *this;


            if(id != 0)
                destroy();
            this->id = other.id;
            other.id = 0;

            return *this;
        }

        Shader(const Shader&) = delete;

        Shader(Shader &&other) noexcept : id(other.id)
        {
            other.id = 0;
        }

        Shader& operator=(const Shader&) = delete;

    private:


};