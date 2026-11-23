#include "Vector.hpp"
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//vector 3 constructors
vector3::vector3(float vx, float vy, float vz)
: x(vx), y(vy), z(vz){};

vector3::vector3(float v)
: x(v), y(v), z(v){};

vector3::vector3(glm::vec3 vec)
: x(vec.x), y(vec.y), z(vec.z) {}


//vector 3 functions
std::string vector3::str()
{
    return "\n{ " + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + " }";
}


//vector 2 constructors

vector2::vector2(float vx, float vy)
: x(vx), y(vy) {};

vector2::vector2(float v)
: x(v), y(v) {};

vector2::vector2(glm::vec2 vec)
: x(vec.x), y(vec.y) {}


//vector 2 functions
std::string vector2::str()
{
    return "\n{ " + std::to_string(x) + "," + std::to_string(y) + " }";
}

namespace Vector 
{
    vector3 Up(0,1,0);
    vector3 Down(0,-1,0);
    vector3 Forward(0,0,-1);
    vector3 Backward(0,0,1);
}
