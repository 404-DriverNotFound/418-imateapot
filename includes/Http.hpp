#pragma once

#include "Method.hpp"
#include "ConfigGroup.hpp"

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
	uint16_t		status_code;
};

class Http
{
	std::map<std::string, std::string>		_headers;
	// TODO: deque를 어떻게 할지
	std::deque<uint8_t>						_body;

	public:
		void insertToHeader(const std::string &, const std::string &);
		std::string getHeaderValue(const std::string &);
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
