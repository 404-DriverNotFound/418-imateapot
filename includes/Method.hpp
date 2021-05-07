#pragma once

#include <string>

#define ERROR -1

enum e_method
{
	HEAD,
	GET,
	PUT,
	POST,
	DELETE
};

int methodToNum(const std::string &);
std::string numToMethod(const e_method);
std::string makeMethodList(bool *methods);
