#pragma once
#include <cstdio>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include "Vector.hpp"
#include "Mesh.hpp"
#include <memory>


class Object
{
    public:
        glm::vec3 position;
        vector3 rotation;
        vector3 scale;

        std::shared_ptr<Mesh> mesh;

        Object() : position(0), rotation(0), scale(1) {};
        Object(std::shared_ptr<Mesh>(pMesh)) : mesh(std::move(pMesh)), position(0), rotation(0), scale(1) {};
        Object(std::shared_ptr<Mesh>(pMesh), glm::vec3 pPosition) : mesh(std::move(pMesh)), position(pPosition), rotation(0), scale(1) {};
        Object(std::shared_ptr<Mesh>(pMesh), vector3 pPosition) : mesh(std::move(pMesh)), position(pPosition), rotation(0), scale(1) {};
        Object(std::shared_ptr<Mesh>(pMesh), glm::vec3 pPosition, vector3 pRotation) : mesh(std::move(pMesh)), position(pPosition), rotation(pRotation), scale(1) {};
        Object(std::shared_ptr<Mesh>(pMesh), glm::vec3 pPosition, vector3 pRotation, vector3 pScale) : mesh(std::move(pMesh)), position(pPosition), rotation(pRotation), scale(pScale) {};
        Object(glm::vec3 pPosition) : position(pPosition), rotation(0), scale(1){};
        Object(vector3 pPosition) : position(pPosition), rotation(0), scale(1){};
        Object(glm::vec3 pPosition, vector3 pRotation) : position(pPosition), rotation(pRotation), scale(1){};
        Object(vector3 pPosition, vector3 pRotation) : position(pPosition), rotation(pRotation), scale(1){};
        Object(glm::vec3 pPosition, vector3 pRotation, vector3 pScale) : position(pPosition), rotation(pRotation), scale(pScale){};
        Object(vector3 pPosition, vector3 pRotation, vector3 pScale) : position(pPosition), rotation(pRotation), scale(pScale){};
        
        Object(Object &&other) noexcept : mesh(std::move(other.mesh)), position(other.position),rotation(other.rotation), scale(other.scale) {}
        Object(const Object&) = delete;
        
        void render(glm::mat4 view,  glm::mat4 projection);

        /*Pure Transformation with no Speed, Great for setting Positions*/
        void changePos(const glm::vec3 movementVector);
        /*Pure Transformation with no Speed, Great for setting Positions*/
        void changePos(const vector3 movementVector);

        /*Pure rotation with N angles yaw, pitch, roll*/
        void changeRotation(const float yawRotation, const float pitchRotation, const float rollRotation);
        /*Pure rotation with N angles yaw, pitch, roll*/
        void changeRotation(const vector3 rotation);

        glm::mat4 getModelMatrix();

        GLuint getUniformLocation(const char *name, const bool cache = true);
        
        template <typename T>
        void setUniform(const char *name, T value, bool useShader = false);

        /*todo: add getModelMatrix implementation
        make uniforms cache
        add uniforms functions
        
        
        */
        private:
        std::map<std::string, GLuint> uniformsLookup;
};