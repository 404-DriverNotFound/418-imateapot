#pragma once

#include "Http.hpp"
#include "Config.hpp"
#include "utils.hpp"
#include "Socket.hpp"
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>

enum e_sock_status
{
	INITIALIZE,
	RECV_START_LINE,
	RECV_HEADER,
	RECV_BODY,
	RECV_END,
	PROC_CGI,
	MAKE_MSG,
	SEND_MSG,
	SEND_DONE
};

enum e_proc_status
{
	PROC_INITIALIZE,
	PROC_READY,
	CREATING,
	SENDING
};

#define EMPTY_CONTENT_LENGTH -1
#define CHUNKED_READY -1

#define PARSE_BODY_END 0
#define PARSE_BODY_LEFT 1

class Client
{
	private:
		int				_fd;
		uint16_t		_port;
		int				_content_length_left;
		int				_chunked_len;
		std::string		_buffer;
		e_sock_status	_sock_status;
		e_proc_status	_proc_status;
		HttpRequest		_request;
		HttpResponse	_response;
		std::string		_file_path;
		Config			*_config_location;

		void makeFilePath();
		void checkFilePath();
		std::string makeContentLocation();

		void makeHeadMsg();
		void makeGetMsg();
		std::string autoindex();
		void makePutMsg();
		void makePostMsg();

		void procCgi();

	public:
		Client(Socket &socket);

		void parseStartLine(const std::string &);
		void parseHeader(const std::string &);
		void setClientResReady(ConfigGroup &group);
		int	parseBody(std::string &, size_t);

		void makeMsg();
		void sendMsg();

		void parseBuffer(char *buff, int len);

		void makeBasicHeader();
		void makeErrorStatus(uint16_t status);

		int				getFd();
		e_sock_status	getSockStatus();
		e_proc_status	getProcStatus();

		class SocketAcceptException: public std::exception
		{
			virtual const char *what() const throw();
		};
};
