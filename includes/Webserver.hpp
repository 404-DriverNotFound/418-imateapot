#pragma once
#include "webserv.hpp"

class Webserver
{
	private:
		ConfigGroup			_configs;
		std::vector<Server>	_servers;
		fd_set				_fd_exception;
		fd_set				_fd_read;
		fd_set				_fd_write;
		std::vector<Socket>	_socks;
		std::vector<Client>	_clients;

	public:
		Webserver(const std::string &, uint32_t max_connection); // config 파일의 경로를 받아서 초기화
		void start_server();	 // 서버 시작
		void accept_request(Socket &sock);


		class SelectException: public std::exception
		{
			virtual const char *what() const throw(); 
		};



	private:
		//void create_server(const ConfigServer &);
};
