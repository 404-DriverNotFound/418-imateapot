#pragma once

#include "ConfigGroup.hpp"
#include "Socket.hpp"
#include "Client.hpp"

#include <vector>
#include <string>
#include <unistd.h>

#define READ_BUFFER 1024

class Webserver
{
	private:
		ConfigGroup			_configs;
		fd_set				_fd_exception;
		fd_set				_fd_read;
		fd_set				_fd_write;
		std::vector<Socket>	_socks;
		std::vector<Client>	_clients;

		void acceptRequest(Socket &sock);
		void readRequest(Client &client);
		void handleResponse(Client &client);

		void handleHttpError(std::map<int, int>& err_index);

	public:
		Webserver(const std::string &path, uint32_t max_connection);
		void startServer();

		/*---- getter ----*/
		fd_set &getFdRead();
		fd_set &getFdWrite();
		fd_set &getFdException();

		class SelectException: public std::exception
		{
			virtual const char *what() const throw();
		};

		class SocketReadException: public std::exception
		{
			virtual const char *what() const throw();
		};
};
