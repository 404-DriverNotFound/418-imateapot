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
	uint8_t		status_code;
};

class Http
{
	std::multimap<std::string, std::string>	_headers;
	// ! 자료 구조에 대해서 상의해야될 것 같습니다. 굳이 uint8_t로 써야하는지?
	std::deque<uint8_t>						_body;

	public:
		void insertToHeader(const std::string &, const std::string &);
		std::string getHeaderValue(const std::string &);
		std::deque<uint8_t>	 &getBody();
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
