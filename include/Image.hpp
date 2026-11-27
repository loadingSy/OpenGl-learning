#pragma once
#include "stb_include.h"
#include "glad/glad.h"
#include <string>

// GLint img1W, img1H, img1ClrChnls;
//     std::string img1Path = getProjectPath("assets/textures/toms.jpg");
//     unsigned char *img1 = stbi_load(getProjectPath("assets/textures/toms.jpg").c_str(), &img1W, &img1H, &img1ClrChnls, 0);
    
//     if(img1 == NULL)
//     {
//         std::cerr << "Image failed to load";
//     }

class Image
{
    public:
        GLuint id;
        unsigned char *imgBuffer;
        Image(std::string imgPath, int setColorChannels, GLint filter = GL_LINEAR);
        ~Image();

        void generateImage(int setColorChannels, GLint imgFilter);
        void generateImage(std::string imgPath, int setColorChannels, GLint imgFilter);
        void bind(GLuint slot);
        void unBind();

        Image(Image &&other) noexcept : id(other.id), imgBuffer(other.imgBuffer), imgPath(other.imgPath), width(other.width), height(other.height), colorChannels(other.colorChannels)
        {
            other.id = 0;
            other.imgBuffer = nullptr;
            other.imgPath = "";
            other.height = 0;
            other.width = 0;
            other.colorChannels = 0;
        }

        std::string imgPath;
        GLint width, height, colorChannels;
    private:
};