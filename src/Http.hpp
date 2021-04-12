#pragma once
#include <string>
#include <map>
#include <deque>
#include "Method.hpp"
#include "Url.hpp"
#include "Path.hpp"

class StartLine
{
	std::string		protocol;
};

class StartLineReq : public StartLine
{
	e_method		method;
	Path			path;
};

class StartLineRes : public StartLine
{
	uint8_t			code;
};

class Http
{
	StartLine									start_line;
	std::multimap<std::string, std::string>		headers;
	std::deque<uint8_t>							body;
};