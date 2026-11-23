#include "../include/Image.hpp"
#include "glad/glad.h"
#include "../stb/stb_image.h"
#include <cstddef>
#include <iostream>



Image::Image(std::string imgPath, int setColorChannels)
: imgPath(imgPath.c_str())
{

    stbi_set_flip_vertically_on_load(1);
    imgBuffer = stbi_load(this->imgPath, &width, &height, &colorChannels, setColorChannels);
    if(imgBuffer == NULL)
    {
        std::cerr << "Image failed to load: " << imgPath;
    }    

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgBuffer);
    glBindBuffer(GL_TEXTURE_2D, 0);
    glGenerateMipmap(id);

    if(imgBuffer)
    {
        stbi_image_free(imgBuffer);
    }
}

Image::~Image()
{
    glDeleteTextures(1, &id);
}

void Image::bind(GLuint slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Image::unBind()
{
    glBindTexture(id, 0);
}