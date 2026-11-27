#include "../include/FileReader.hpp"
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

template <>
std::vector<float> readFileToVector(const std::string &filePath, char delimiter)
{
    return std::vector<float>();
}

template <>
std::vector<int> readFileToVector(const std::string &filePath, char delimiter)
{
    std::fstream file(filePath, std::ios::in);
    if(!file.is_open())
    {
        std::cout << "\nError: couldnt read file(int) \"" << filePath << "\"\n";
        return std::vector<int>();
    }

    std::vector<int> dataVec;
    std::string buffer;

    char c = ' ';
    while (file.get(c)) 
    {
        if (c == delimiter || c == ' ' || c == '\n' || c == '\r')
        {
            if(!buffer.empty())
            {
                if(buffer == "+" || buffer == "-")
                {
                    std::cout << "\nFound non digit in \"" << filePath << "\"\n";
                    return dataVec;
                }
                
                dataVec.push_back(std::stoul(buffer));
                buffer.clear();
            }
            
            continue;
        }
        if(!std::isdigit(static_cast<unsigned char>(c)) && ((c != '+' && c != '-' ) || !buffer.empty()))
        {
            std::cout << "\nFound non digit in \"" << filePath << "\"\n";
            return dataVec;
        }
        buffer += c; 
    }
    if(!buffer.empty())
                dataVec.push_back(std::stoi(buffer));
    if(file.eof())
        std::cout << "\nRead complete Successfully, \"" << filePath << "\"\n";

    return dataVec;
}

template <>
std::vector<unsigned int> readFileToVector(const std::string &filePath, char delimiter)
{
    std::fstream file(filePath, std::ios::in);
    if(!file.is_open())
    {
        std::cout << "\nError: couldnt read file(uint) \"" << filePath << "\"\n";
        return std::vector<unsigned int>();
    }

    std::vector<unsigned int> dataVec;
    std::string buffer;

    char c = ' ';
    while (file.get(c)) 
    {
        if (c == delimiter || c == ' ' || c == '\n' || c == '\r')
        {
            if(!buffer.empty())
            {
                dataVec.push_back(std::stoul(buffer));
                buffer.clear();
            }
            
            continue;
        }
        if(!std::isdigit(static_cast<unsigned char>(c)))
        {
            std::cout << "\nFound non digit in \"" << filePath << "\"\n";
            return dataVec;
        }
        buffer += c; 
    }
    if(!buffer.empty())
                dataVec.push_back(std::stoi(buffer));
    if(file.eof())
        std::cout << "\nRead complete Successfully, \"" << filePath << "\"\n";

    return dataVec;
    
}