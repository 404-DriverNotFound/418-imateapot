#pragma once
#ifdef __linux__
	#include <stdint.h>
#endif
#include "ConfigGroup.hpp"
#include "Path.hpp"
#include "Server.hpp"
#include <sys/select.h>
#include <vector>

class Webserver
{
	ConfigGroup			_configs;
	std::vector<Server>	_servers;

	// 이름 좀 바꿉시다..
	fd_set				to_be_checked;
	fd_set				to_be_checked_read;
	fd_set				to_be_checked_write;

	public:
		Webserver(const std::string &, uint32_t max_connection); // config 파일의 경로를 받아서 초기화
		void start_server();	 // 서버 시작

	//private:
		//void create_server(const ConfigServer &);
};
