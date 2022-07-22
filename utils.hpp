#pragma once

#include <vector>
#include <cstring>
#include <iostream>

typedef std::vector<std::string> StringVector;

StringVector split(std::string line, std::string delimiter);