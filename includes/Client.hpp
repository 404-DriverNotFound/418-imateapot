#pragma once

#include "Http.hpp"
#include "Config.hpp"
#include "utils.hpp"
#include "Socket.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

enum e_sock_status
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

enum e_proc_status
{
	PROC_INITIALIZE,
	CREATING,
	SENDING
};

class Client
{
	private:
		int				_fd;
		uint16_t		_port;
		std::string		_buffer;
		e_sock_status	_sock_status;
		e_proc_status	_proc_status;
		HttpRequest		_request;
		HttpResponse	_response;
		Config			*_config_location;

		bool makeHeadMsg();
		void makeGetMsg();
		void makePutMsg();
		void makePostMsg();

		void procCgi();

	public:
		Client(Socket &socket);

		void parseStartLine(const std::string &);
		void parseHeader(const std::string &);
		void setConfig(ConfigGroup &group);
		void parseBody();

		void makeMsg();
		void sendMsg();

		void parseBuffer(char *buff, int len);

		int				getFd();
		e_proc_status	getProcStatus();

		class SocketAcceptException: public std::exception
		{
			virtual const char *what() const throw(); 
		};

		class RequestFormatException: public std::exception
		{
			virtual const char *what() const throw(); 
		};
};
