#pragma once
#include <string>
#include <vector>


std::string readFullFile(std::string filePath);

template <typename T>
std::vector<T> readFileToVector(const std::string &filePath, char delimiter = ' ');


template <>
std::vector<float> readFileToVector(const std::string &filePath, char delimiter);

template <>
std::vector<int> readFileToVector(const std::string &filePath, char delimiter);

template <>
std::vector<unsigned int> readFileToVector(const std::string &filePath, char delimiter);