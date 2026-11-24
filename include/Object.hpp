#include <cstdio>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Vector.hpp"
#include "Mesh.hpp"


class Object
{
    public:
        glm::vec3 position;
        vector2 rotation;

        Mesh mesh;

        Object() : position(0), rotation(0) {};
        Object(Mesh &&pMesh) : mesh(std::move(pMesh)), position(0), rotation(0) {};
        Object(Mesh &&pMesh, glm::vec3 pPosition) : mesh(std::move(pMesh)), position(pPosition), rotation(0) {};
        Object(Mesh &&pMesh, vector3 pPosition) : mesh(std::move(pMesh)), position(pPosition), rotation(0) {};
        Object(Mesh &&pMesh, glm::vec3 pPosition, vector2 pRotation) : mesh(std::move(pMesh)), position(pPosition), rotation(pRotation) {};
        Object(glm::vec3 pPosition) : position(pPosition), rotation(0){};
        Object(vector3 pPosition) : position(pPosition), rotation(0){};
        Object(glm::vec3 pPosition, vector2 pRotation) : position(pPosition), rotation(pRotation){};
        Object(vector3 pPosition, vector2 pRotation) : position(pPosition), rotation(pRotation){};

        void render();

        /*Pure Transformation with no Speed, Great for setting Positions*/
        void changePos(glm::vec3 movementVector);
        /*Pure Transformation with no Speed, Great for setting Positions*/
        void changePos(vector3 movementVector);

        /*Pure rotation with N angles yaw, pitch*/
        void changeRotation(float yawRotation, float pitchRotation);
        /*Pure rotation with N angles yaw, pitch*/
        void changeRotation(vector2 rotation);

        glm::vec4 getModelMatrix();
};