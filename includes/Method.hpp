#pragma once
#include <string>

enum e_method
{
	GET,
	HEAD,
	PUT,
	POST,
	TRACE,
	DELETE,
};

int methodToNum(const std::string &);
std::string numToMethod(const e_method);
