#pragma once
#ifdef __linux__
	#include <stdint.h>
#endif
class Client;
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
	Socket sock;
	std::string buffer;
	e_status status;
	Http req;
	Http res;
	Config &config_location;

public:
	void recv_start_line();
	void recv_header();
	void recv_body();
	void proc_cgi();
	void make_msg();
	void send_msg();
};
