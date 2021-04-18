#pragma once

#include "Http.hpp"
#include "Config.hpp"
#include "utils.hpp"
#include "Socket.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

enum e_status
{
	INITIALIZE,
	RECV_START_LINE,
	RECV_HEADER,
	RECV_BODY,
	PROC_CGI,
	MAKE_MSG,
	SEND_MSG,
	SEND_DONE
};

class Client
{
	private:
		int				_fd;
		uint16_t		_port;
		std::string		_buffer;
		e_status		_status;
		HttpRequest		_request;
		HttpResponse	_response;
		Config			*_config_location;

	public:
		Client(Socket &socket);

		void recvStartLine(const std::string &);
		void recvHeader();
		void recvBody();
		void procCgi();
		void makeMsg();
		void sendMsg();

		void appendBuffer(char *buff, int len);
		void parseBuffer();

		int getFd();

		class SocketAcceptException: public std::exception
		{
			virtual const char *what() const throw(); 
		};

		class RequestFormatException: public std::exception
		{
			virtual const char *what() const throw(); 
		};
};
