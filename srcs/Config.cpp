#include "webserv.hpp"

/**
 * Config::Config
 * @brief  Server Default Config에서 사용하는 생성자
 */
Config::Config():
	port(80),
	body_length(1000000),
	autoindex(false),
	timeout(5)
{
	method[0] = method[1] = true;
	method[2] = method[3] = false;
}

/**
 * Config::Config
 * @brief  Location Config에서 사용하는 (복사)생성자
 * @param  {Config} src           : 복사할 Server Default Config
 * @param  {std::string} loc_path : Location 경로
 */
Config::Config(const Config &src, std::string &loc_path):
	server_name(src.server_name),
	root(src.root),
	port(src.port),
	index(src.index),
	error_page(src.error_page),
	body_length(src.body_length),
	autoindex(src.autoindex),
	timeout(src.timeout),
	auth(src.auth),
	cgi_path(src.cgi_path),
	cgi_extension(src.cgi_extension),
	server_root(src.server_root),
	location_path(loc_path)
{
	for (int i = 0; i < 4; i++)
		method[i] = src.method[i];
}

/**
 * Config::parseConfig
 * @brief  설정 파일의 한줄을 key, value로 쪼개진 vector로 받아와 이를 config 인스턴스에 적용하는 함수
 * @param  {std::vector<std::string>} split : key, value로 쪼개진 설정값
 * @param  {bool} is_location               : Location Config일 경우 true, Server 전체 Config일 경우 false
 */
void Config::parseConfig(std::vector<std::string> &split, bool is_location)
{
	if (split.size() < 2)
		throw ConfigGroup::ConfigFormatException();
	if (!split[0].compare("server_name"))
	{
		if (is_location)
			throw ConfigGroup::ConfigFormatException();
		this->server_name = split[1];
	}
	else if (!split[0].compare("port"))
	{
		if (is_location)
			throw ConfigGroup::ConfigFormatException();
		this->port = ft_atoi(split[1]);
	}
	else if (!split[0].compare("index"))
	{
		this->index = this->root + "/" + split[1];
	}
	else if (!split[0].compare("error_page"))
	{
		this->error_page = this->root + "/" + split[1];
	}
	else if (!split[0].compare("body_length"))
	{
		this->body_length = ft_atoi(split[1]);
	}
	else if (!split[0].compare("autoindex"))
	{
		if (!split[1].compare("on"))
			this->autoindex = true;
		else if (!split[1].compare("off"))
			this->autoindex = false;
		else
			throw ConfigGroup::ConfigFormatException();
	}
	else if (!split[0].compare("timeout"))
	{
		this->timeout = ft_atoi(split[1]);
	}
	else if (!split[0].compare("auth"))
	{
		this->auth = split[1];
	}
	else if (!split[0].compare("method"))
	{
		if (is_location)
			for (int i = 0; i < 4; i++)
				this->method[i] = false;
		for (size_t i = 1; i < split.size(); i++)
		{
			if (split[i].compare("GET"))
				this->method[GET] = true;
			else if (split[i].compare("HEAD"))
				this->method[HEAD] = true;
			else if (split[i].compare("PUT"))
				this->method[PUT] = true;
			else if (split[i].compare("POST"))
				this->method[POST] = true;
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
			this->index = this->root + "/index.html";
			this->error_page = this->root + "/error.html";
		}
	}
	else if (!split[0].compare("cgi_path"))
	{
		this->cgi_path = split[1];
	}
	else if (!split[0].compare("cgi_extension"))
	{
		this->cgi_extension = split[1];
	}
	else
		throw ConfigGroup::ConfigFormatException();
}
