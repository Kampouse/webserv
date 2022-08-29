#pragma once

#include <vector>
#include <cstring>
#include <iostream>
#include <sstream>

typedef std::vector<std::string> StringVector;

StringVector split(std::string line, std::string delimiter);

// Convert Numerical type to String
template <typename T>
std::string IntToString(const T& num) {
    std::stringstream ss;
    std::string str;

    ss << num;
    ss >> str;

    return str;
}