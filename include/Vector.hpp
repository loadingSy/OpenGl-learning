#pragma once
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class vector3
{
    public:
    float x;
    float y;
    float z;

    std::string str();
    vector3(float vx, float vy, float vz);
    vector3(float v);
    vector3(glm::vec3 vec);


    static const vector3 Up;
    static const vector3 Right;
    static const vector3 Forward;


    vector3 operator+(const vector3 &v)
    {
        return vector3(v.x + x, v.y + y, v.z + z);
    };
    vector3 operator+(const glm::vec3 &v)
    {
        return vector3(v.x + x, v.y + y, v.z + z);
    };
    vector3 operator*(const vector3 &v)
    {
        return vector3(v.x * x, v.y * y, v.z * z);
    };
    vector3 operator*(const glm::vec3 &v)
    {
        return vector3(v.x * x, v.y * y, v.z * z);
    };
    vector3& operator+=(const glm::vec3 &v)
    {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
        return *this;
    }
    vector3& operator+=(const vector3 &v)
    {
        this->x += v.x;
        this->y += v.y;
        this->z += v.z;
        return *this;
    }
    
    operator glm::vec3()const {
        return glm::vec3(x,y,z);
    }
    
};



class vector2
{
    public:
    float x;
    float y;

    std::string str();
    vector2(float vx, float vy);
    vector2(float v);
    vector2(glm::vec2 vec);

    vector2 operator+(const vector2 &v)
    {
        return vector2(v.x + x, v.y + y);
    };
    vector2 operator+(const glm::vec2 &v)
    {
        return vector2(v.x + x, v.y + y);
    };
    vector2 operator*(const vector2 &v)
    {
        return vector2(v.x * x, v.y * y);
    };
    vector2 operator*(const glm::vec2 &v)
    {
        return vector2(v.x * x, v.y * y);
    };
    vector2& operator+=(const glm::vec2 &v)
    {
        this->x += v.x;
        this->y += v.y;
        return *this;
    }
    vector2& operator+=(const vector2 &v)
    {
        this->x += v.x;
        this->y += v.y;
        return *this;
    }
    
    operator glm::vec2()const {
        return glm::vec2(x,y);
    }
    
};


