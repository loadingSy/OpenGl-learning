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

    private:


};