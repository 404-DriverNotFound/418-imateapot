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
	MAKE_MSG,
	SEND_MSG,
	SEND_DONE
};

#define BUF_SIZE 65535

#define EMPTY_CONTENT_LENGTH -1
#define CHUNKED_READY -1

#define PARSE_BODY_END 0
#define PARSE_BODY_LEFT 1

#define CLIENT_DONE_STATUS 1000

class Webserver;

class Client
{
	private:
		uint16_t		_port;
		int				_fd;
		int				_content_length_left;
		int				_chunked_length;
		std::string		_buffer;
		e_sock_status	_sock_status;
		HttpRequest		_request;
		HttpResponse	_response;
		std::string		_file_path;
		Config			*_config_location;
		Socket			*_socket;
		bool			_is_read_finished;

		void makeFilePath();
		void checkFilePath();
		std::string makeContentLocation();
		std::string makeAutoindex();

		void makeHeadMsg();
		void makeGetMsg();
		void makePutMsg();
		void makePostMsg();

		bool isCGIRequest();
		char **setEnv();
        void execCGI();
		void parseCGIBuffer(std::string &temp_string, bool &is_header_finished);

	public:
		Client(Socket &socket, int fd);

		void parseStartLine(const std::string &);
		void parseHeader(const std::string &);
		void setClientResReady(ConfigGroup &group);
		int	parseBody();

		void makeMsg();
		void sendMsg();

		void parseBuffer(char *buff, int len, ConfigGroup &configs);

		void makeBasicHeader();
		void makeErrorStatus(uint16_t status);

		bool isConfigSet();
		void reset();

		int				getFd();
		e_sock_status	getSockStatus();
		bool			getIsReadFinished();

		void			setIsReadFinished(bool is_read_finished);
		

		class SocketAcceptException: public std::exception
		{
			virtual const char *what() const throw();
		};
};
