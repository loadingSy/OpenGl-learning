#include "../include/FileReader.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string readFullFile(std::string filePath)
{
    std::fstream file(filePath);
    if(!file.is_open())
    {
        std::cerr << "\nFile Failed To Open:" << filePath;
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    
    file.close();

    return ss.str();
};