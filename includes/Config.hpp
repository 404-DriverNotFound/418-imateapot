#pragma once

#ifdef __linux
	#include <stdint.h>
#endif
#include <string>
#include <vector>
#include <cstdlib> // exit() 사용하지 않게 되었을 때 삭제해주세요!

struct Config
{
	std::string		server_name;
	std::string		root;
	uint16_t		port;				// def = 80;
	std::string		index;				// def = server_root/index.html
	std::string		error_page;			// def = server_root/error.html
	uint64_t		head_length;		// def = 8k
	uint64_t		body_length;		// def = 1M
	bool			autoindex;			// def = off
	uint32_t		timeout;			// def = 5s
	std::string		auth;
	bool			method[4];			// def = GET HEAD
	std::string		cgi_path;
	std::string		cgi_extension;
	std::string		server_root;
	std::string		location_path;

	Config();
	Config(const Config &, std::string &);

	void parseConfig(std::vector<std::string> &split, bool is_location);
};
