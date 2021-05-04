#pragma once

#include "ConfigGroup.hpp"
#include "Socket.hpp"
#include "Client.hpp"

#include <vector>
#include <string>
#include <unistd.h>

#define CLIENT_END 0
#define CLIENT_CONTINUE 1

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
		int readRequest(Client &client);
		void handleResponse(Client &client);

		void handleClientDone(std::map<int, int>& done_info);

	public:
		Webserver(const std::string &path, uint32_t max_connection);
		~Webserver();
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
