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
        void use();
        void unUse();
        void destroy();

        Shader &operator=(Shader&& other)
        {
            if(this == &other) return *this;

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