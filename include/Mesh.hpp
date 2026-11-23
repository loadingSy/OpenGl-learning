#pragma once
#include "glad/glad.h"
#include "VBO.hpp"
#include "EBO.hpp"
#include "VAO.hpp"
#include "Shader.hpp"
#include <string>

class Mesh
{
    public:
        GLuint shaderId;
        GLuint vboId;
        GLuint vaoId;
        GLuint eboId;

        void assignVertices(const GLfloat* vertices, GLsizeiptr size);
        void assignIndices(const GLuint *Indices, GLsizeiptr size);
        void assignShader(std::string vShader, std::string fShader);
        
        void linkAttribute(GLuint layout, GLint numComponents, GLenum type, GLsizei stride, const void* offset); 

        Mesh(const GLfloat* vertices, GLsizeiptr verticesSize, const GLuint *Indices, GLsizeiptr size, std::string vShader, std::string fShader);
        Mesh(std::string vShader, std::string fShader);
        Mesh(const GLfloat* vertices, GLsizeiptr verticesSize, const GLuint *Indices, GLsizeiptr size);
        Mesh();
        
        GLuint getUniformLocation(std::string name);

    Shader *getShader();
    VAO *getVAO();
    VBO *getVBO();
    EBO *getEBO();

    private:
        Shader shader;
        VAO vao;
        VBO vbo;
        EBO ebo;

        void genVao();
};