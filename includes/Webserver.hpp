#pragma once
#include "ConfigGroup.hpp"
#include "Path.hpp"
#include "Server.hpp"
#include <sys/select.h>
#include <vector>

class Webserver
{
	ConfigGroup		config;
	fd_set				to_be_checked;
	fd_set				to_be_checked_read;
	fd_set				to_be_checked_write;
	std::vector<Server>	servers;

	public:
		Webserver(const Path &); // config 파일의 경로를 받아서 초기화
		void start_server();	 // 서버 시작

	private:
		void parse_config(const Path &); // config 파일 해석
		//void create_server(const ConfigServer &);
};
