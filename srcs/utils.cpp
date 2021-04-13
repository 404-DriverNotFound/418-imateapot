#include "utils.hpp"

std::vector<std::string> ft_split(std::string str, char delim)
{
	std::vector<std::string> answer;
    std::stringstream ss(str);
    std::string temp;
 
    while (std::getline(ss, temp, delim))
        answer.push_back(temp);
    return answer;
}