#include "utils.hpp"

StringVector split(std::string line, std::string delimiter)
{
	StringVector result;

	if (line.empty())
		return result;

	size_t i = 0, j = 0;

	while (i < line.length() && j < line.length())
	{
		i = line.find_first_of(delimiter, j);
		if (i == std::string::npos)
			i = line.length();
		std::string str = line.substr(j, i - j);
		if (!str.empty())
			result.push_back(str);
		j = line.find_first_not_of(delimiter, i);
	}
	return result;
}

