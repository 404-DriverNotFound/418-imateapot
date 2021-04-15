#pragma once
#ifdef __linux__
	#include <stdint.h>
#endif
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> ft_split(std::string str, char delim);
