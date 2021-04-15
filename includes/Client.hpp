#pragma once
#include "Socket.hpp"
#include "Http.hpp"
#include <sys/types.h>
#include "Config.hpp"

#define BUFFER_SIZE 16

enum e_status
{
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
		Socket			_sock;
		std::string		_buffer;
		e_status		_status;
		HttpRequest		_request;
		HttpResponse	_response;
		Config			&_config_location;

	public:
		void recvStartLine(const std::string &);
		void recvHeader();
		void recvBody();
		void procCgi();
		void makeMsg();
		void sendMsg();

		class RequestFormatException: public std::exception
		{
			virtual const char *what() const throw(); 
		};
};
