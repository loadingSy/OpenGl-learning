#include "glad/glad.h"
#include "Mesh.hpp"




Mesh::Mesh(const GLfloat* vertices, GLsizeiptr verticesSize, const GLuint *Indices, GLsizeiptr size, std::string vShader, std::string fShader)
: Mesh{vertices, verticesSize, Indices, size}//delegate constructors
{
    vaoId = vao.id;
    assignShader(vShader, fShader);
}
Mesh::Mesh(std::string vShader, std::string fShader)
{
    vaoId = vao.id;
    assignShader(vShader, fShader);
}
Mesh::Mesh(const GLfloat* vertices, GLsizeiptr verticesSize, const GLuint *Indices, GLsizeiptr size)
{
    vaoId = vao.id;
    assignIndices(Indices, size);
    assignVertices(vertices, verticesSize);
}
Mesh::Mesh()
{
    vaoId = vao.id;
}
Shader *Mesh::getShader()
{
    return &shader;
}
VAO *Mesh::getVAO()
{
    return &vao;
}
VBO *Mesh::getVBO()
{
    return &vbo; 
}
EBO *Mesh::getEBO()
{
    return &ebo;
}

void Mesh::assignShader(std::string vShader, std::string fShader)
{
    shader.assignShader(vShader, fShader);
    shaderId = shader.id;
}
void Mesh::assignIndices(const GLuint *Indices, GLsizeiptr size)
{
    iSize = size;
    vao.bind();
    ebo.setData(Indices, size);
    ebo.bind();
    vao.unBind();
    ebo.unBind();

    eboId = ebo.id;
}
void Mesh::assignVertices(const GLfloat* vertices, GLsizeiptr size)
{
    vSize = size;
    vao.bind();
    vbo.setData(vertices, size);
    vbo.bind();
    vao.unBind();
    vbo.unBind();
    vboId = vbo.id;
}
void Mesh::linkAttribute(GLuint layout, GLint numComponents, GLenum type, GLsizei stride, const void* offset)
{
    vao.linkAttribute(vbo, layout, numComponents, type, stride, offset);
}
GLuint Mesh::getUniformLocation(std::string name)
{
    return glGetUniformLocation(shaderId, name.c_str());
}

GLsizeiptr Mesh::getVsize()
{
    return vSize;
}
GLsizeiptr Mesh::getIsize()
{
    return iSize;
}