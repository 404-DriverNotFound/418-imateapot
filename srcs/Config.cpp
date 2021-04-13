#include "Config.hpp"

Config::Config():
	port(80),
	index_page("index.html"),
	error_page("error.html"),
	head_length_max(8000),
	body_length_max(1000000),
	autoindex(false),
	timeout(5)
{
	methods[0] = methods[1] = true;
	methods[2] = methods[3] = methods[4] = methods[5] = false;
}

Config::Config(const Config &src):
	server_name(src.server_name),
	root(src.root),
	port(src.port),
	index_page(src.index_page),
	error_page(src.error_page),
	head_length_max(src.head_length_max),
	body_length_max(src.body_length_max),
	autoindex(src.autoindex),
	timeout(src.timeout),
	auth(src.auth)
{
	for (int i = 0; i < 6; i++)
		methods[i] = src.methods[i];
}