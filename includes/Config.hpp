#pragma once
#include "Method.hpp"
#include <string>

struct Config
{
	std::string		server_name;
	std::string		root;
	uint16_t		port;				// def = 80;
	std::string		index_page;			// def = index.html
	std::string		error_page;			// def = error.html
	uint64_t		head_length_max;	// def = 8k
	uint64_t		body_length_max;	// def = 1M
	bool			autoindex;			// def = off
	uint32_t		timeout;			// def = 5s
	std::string		auth;
	bool			methods[6];			// def = GET HEAD
	std::string		location_path;

	Config();
	Config(const Config &src);
};
