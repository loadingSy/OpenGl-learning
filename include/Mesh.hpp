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
        Mesh(Shader &&pShader, VAO &&pVao, VBO &&pVbo, EBO &&pEbo);
        Mesh();


        Mesh(Mesh&& other) noexcept : shaderId(other.shaderId), vaoId(other.vaoId), vboId(other.vboId), eboId(other.eboId), shader(std::move(other.shader)), vao(std::move(other.vao)), vbo(std::move(other.vbo)), ebo(std::move(other.ebo)), vSize(other.vSize), iSize(other.iSize)
        {
            other.shaderId = 0;
            other.shader.id = 0;

            other.vaoId = 0;
            other.vao.id = 0;
            
            other.vboId = 0;
            other.vbo.id = 0;

            other.eboId = 0;
            other.ebo.id = 0;

            other.vSize = 0;
            other.iSize = 0;
        }

        //this is the destination (the left side)
        //v is the source
        Mesh& operator=(Mesh &&v)
        {
            if (this == &v) return *this;

            this->eboId = v.eboId;
            this->shaderId = v.shaderId;
            this->vboId = v.vboId;
            this->vaoId = v.vaoId;

            this->vSize = v.vSize;
            this->iSize = v.iSize;

            v.ebo.id = 0;
            v.eboId = 0;

            v.shader.id = 0;
            v.shaderId = 0;

            v.vao.id = 0;
            v.vaoId = 0;

            v.vbo.id = 0;
            v.vboId = 0;

            v.vSize = 0;
            v.iSize = 0;
            
            return *this;
        }

        
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
        

        GLuint getUniformLocation(std::string name);

    GLsizeiptr getVsize();
    GLsizeiptr getIsize();
    Shader *getShader();
    VAO *getVAO();
    VBO *getVBO();
    EBO *getEBO();

    private:
        GLsizeiptr vSize;
        GLsizeiptr iSize;
        Shader shader;
        VAO vao;
        VBO vbo;
        EBO ebo;

        void genVao();
};