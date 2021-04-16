#pragma once

#include <string>

#define ERROR -1

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
