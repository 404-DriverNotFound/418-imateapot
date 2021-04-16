#pragma once

#include "Method.hpp"

#include <map>
#include <deque>

struct StartLine
{
	std::string protocol;
};

struct StartLineReq : public StartLine
{
	e_method	method;
	std::string	path;
	std::string query_string;
};

struct StartLineRes : public StartLine
{
	uint8_t		status_code;
};

class Http
{
	std::multimap<std::string, std::string>	_headers;
	std::deque<uint8_t>						_body;

	public:
		void insertToHeader(const std::string &, const std::string &);
};

class HttpRequest : public Http
{
	StartLineReq			_start_line;

	public:
		StartLineReq &getStartLine();
};

class HttpResponse : public Http
{
	StartLineRes			_start_line;

	public:
		StartLineRes &getStartLine();
};
