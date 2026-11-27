#include "Image.hpp"
#include "Object.hpp"
#include <initializer_list>
#include <string>
#include <string_view>
#include <utility>
#include <vector>


class Box
{
    public:
    Object obj;
    std::vector<int> attribsList;
    std::string name;

    Image image;
    bool hasImage = false;

    Box(Object &&pObj, std::initializer_list<int> pAttribs) : obj(std::move(pObj)), attribsList(pAttribs), image(Image("", 0)), hasImage(false), name("Box") {}

    Box(Object &&pObj, std::initializer_list<int> pAttribs, Image pImg) : obj(std::move(pObj)), attribsList(pAttribs), image(Image(std::move(pImg))), hasImage(true) {}

    Box(Object &&pObj, std::initializer_list<int> pAttribs, std::string_view pName) : obj(std::move(pObj)), attribsList(pAttribs), name(pName), image(Image("", 0)), hasImage(false) {}

};