#include "Box.hpp"




void Box::render(const glm::mat4 view, const glm::mat4 projection)
{
    obj.setUniform("hasImage", hasImage, true);
    if(isLit)
    {
        obj.setUniform("lightColor", glm::vec3(0.5,0.5,0.5));
    }
    obj.render(view, projection);
}