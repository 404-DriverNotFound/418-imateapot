#include "Config.hpp"
#include "ConfigGroup.hpp"

Config::Config():
	port(80),
	index("index.html"),
	error_page("error.html"),
	head_length(8000),
	body_length(1000000),
	autoindex(false),
	timeout(5)
{
	method[0] = method[1] = true;
	method[2] = method[3] = method[4] = method[5] = false;
}

Config::Config(const Config &src, std::string &loc_path):
	server_name(src.server_name),
	root(src.root),
	port(src.port),
	index(src.index),
	error_page(src.error_page),
	head_length(src.head_length),
	body_length(src.body_length),
	autoindex(src.autoindex),
	timeout(src.timeout),
	auth(src.auth),
	server_root(src.server_root),
	location_path(loc_path)
{
	for (int i = 0; i < 6; i++)
		method[i] = src.method[i];
}

void Config::parseConfig(std::vector<std::string> &split, bool is_location)
{
	if (!split[0].compare("server_name"))
	{
		if (is_location)
			throw ConfigGroup::ConfigFormatException();
		this->server_name = split[1];
		// TODO: 복수의 server_name이 들어오는 것을 허용할지
		// 금요일에 이야기해보기
	}
	else if (!split[0].compare("port"))
	{
		if (is_location)
			throw ConfigGroup::ConfigFormatException();
		this->port = std::atoi(split[1].c_str());
	}
	else if (!split[0].compare("index"))
	{
		this->index = split[1];
	}
	else if (!split[0].compare("error_page"))
	{
		this->error_page = split[1];
	}
	else if (!split[0].compare("head_length"))
	{
		this->head_length = std::atoi(split[1].c_str());
	}
	else if (!split[0].compare("body_length"))
	{
		this->head_length = std::atoi(split[1].c_str());
	}
	else if (!split[0].compare("autoindex"))
	{
		if (split[1].compare("on"))
			this->autoindex = true;
		else if (split[1].compare("off"))
			this->autoindex = false;
		else
			throw ConfigGroup::ConfigFormatException();
	}
	else if (!split[0].compare("timeout"))
	{
		this->timeout = std::atoi(split[1].c_str());
	}
	else if (!split[0].compare("auth"))
	{
		this->auth = split[1];
	}
	else if (!split[0].compare("method"))
	{
		if (is_location)
			for (int i = 0; i < 6; i++)
				method[i] = false;
		for (int i = 1; i < split.size(); i++)
		{
			if (split[i].compare("GET"))
				this->method[GET] = true;
			else if (split[i].compare("HEAD"))
				this->method[HEAD] = true;
			else if (split[i].compare("PUT"))
				this->method[PUT] = true;
			else if (split[i].compare("POST"))
				this->method[POST] = true;
			else if (split[i].compare("TRACE"))
				this->method[TRACE] = true;
			else if (split[i].compare("DELETE"))
				this->method[DELETE] = true;
			else
				throw ConfigGroup::ConfigFormatException();
		}
	}
	else if (!split[0].compare("root"))
	{
		if (is_location)
		{
			this->root = this->server_root;
			this->root.append("/");
			this->root.append(split[1]);
		}
		else
		{
			this->root = split[1];
			this->server_root = this->root;
		}
	}
	else
		throw ConfigGroup::ConfigFormatException();
}
