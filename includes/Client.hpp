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
#include <string>

enum e_sock_status
{
	INITIALIZE,
	RECV_START_LINE,
	RECV_HEADER,
	RECV_BODY,
	MAKE_READY,
	PROC_CGI_HEADER,
	PROC_CGI_BODY,
	MAKE_MSG,
	SEND_HEADER,
	SEND_BODY,
	SEND_DONE
};

#define BUF_SIZE 65535

#define EMPTY_CONTENT_LENGTH -1
#define CHUNKED_READY -1

#define PARSE_BODY_END 0
#define PARSE_BODY_LEFT 1

#define CLIENT_DONE_STATUS 1000

#define CGI_NONE 0
#define CGI_PHP 1
#define CGI_CUSTOM 2

class Webserver;

class Client
{
	private:
		pid_t			_pid;
		uint16_t		_port;
		int				_fd;
		int				_read_fd;
		int				_write_fd;
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
		void makeDeleteMsg();

		int isCGIRequest();
		char **setEnv(int cgi_type);
        void execCGI();
		void freeEnv(char **env);
		void parseCGIBuffer();

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

		void readData(fd_set &fd_read_set);
		void writeData(fd_set &fd_write_set);

		bool isConfigSet();
		void reset(fd_set &fd_read_set, fd_set &fd_write_set);

		int				getFd();
		int				getReadFd();
		int				getWriteFd();

		e_sock_status	getSockStatus();
		bool			getIsReadFinished();

		void			setIsReadFinished(bool is_read_finished);
		void			setReadFd(int fd);
		void			setWriteFd(int fd);
		

		class SocketAcceptException: public std::exception
		{
			virtual const char *what() const throw();
		};
};
