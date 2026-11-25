#include "../include/Shader.hpp"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "../include/FileReader.hpp"

Shader::Shader(std::string vertexShaderFile, std::string fragmentShaderFile)
{
    assignShader(vertexShaderFile, fragmentShaderFile);
}
Shader::Shader()
{

}

void Shader::assignShader(std::string vertexShaderFile, std::string fragmentShaderFile)
{
    //loading shaders using file reading
    std::string fragmentSourceCode = readFullFile(fragmentShaderFile);
    std::string vertexSourceCode = readFullFile(vertexShaderFile);
    
    //checking if shaders loaded
    if(fragmentSourceCode.empty() || vertexSourceCode.empty())
    {
        std::cerr << "Failed To Load Shader\nFile Either Empty Or Not Found";
        return;
    }

    //transforming into a c string
    const char *fSourceCode, *vSourceCode;
    vSourceCode = vertexSourceCode.c_str();
    fSourceCode = fragmentSourceCode.c_str();

    GLuint vertexShader, fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vSourceCode, NULL);
    glShaderSource(fragmentShader, 1, &fSourceCode, NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    GLint fShaderCompiled = 0, vShaderCompiled = 0;

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);

    if(fShaderCompiled == GL_FALSE || vShaderCompiled == GL_FALSE)
    {
        if(fShaderCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, errorLog.data());

            std::cerr << "Fragment Shader Compilation Error:\n" << errorLog.data() << std::endl;
            glDeleteShader(fragmentShader);
        }
        if(vShaderCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> errorLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, errorLog.data());

            std::cerr << "Vertex Shader Compilation Error:\n" << errorLog.data() << std::endl;
            glDeleteShader(vertexShader);
        }
    }
    
    if (!id)
        id = glCreateProgram();

    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);

    glLinkProgram(id);

    glUseProgram(id);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

Shader::~Shader()
{
    glDeleteProgram(id);
}

const void Shader::use()
{
    glUseProgram(id);
}

const void Shader::unUse()
{
    glUseProgram(0);
}

void Shader::destroy()
{
    glDeleteProgram(id);
}


GLuint Shader::getUniformLocation(const char* name)
{
    return glGetUniformLocation(id, name);
}