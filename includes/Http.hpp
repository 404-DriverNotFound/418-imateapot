#pragma once

#include "Method.hpp"
#include "ConfigGroup.hpp"

#include <map>
#include <deque>
#include <unistd.h>

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
	StartLineRes();
};

class Http
{
	std::map<std::string, std::string>		_headers;
	std::string								_body;

	public:
		void insertToHeader(const std::string &, const std::string &);
		std::string getHeaderValue(const std::string &);
		std::string &getBody();
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
		void sendStartLine(int fd);
		void sendHeader(int fd);
		void sendBody(int fd);
		StartLineRes &getStartLine();
};

std::ostream &operator<<(std::ostream &o, StartLineReq &rhs);
