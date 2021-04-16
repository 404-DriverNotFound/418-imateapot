#pragma once
#ifdef __linux__
	#include <stdint.h>
#endif
#include "ConfigGroup.hpp"
#include "Server.hpp"
#include <sys/select.h>
#include <vector>

class Webserver
{
	ConfigGroup			_configs;
	std::vector<Server>	_servers;
	fd_set				_fd_exception;
	fd_set				_fd_read;
	fd_set				_fd_write;

	public:
		Webserver(const std::string &, uint32_t max_connection); // config 파일의 경로를 받아서 초기화
		void start_server();	 // 서버 시작

	private:
		//void create_server(const ConfigServer &);
};
