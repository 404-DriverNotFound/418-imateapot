#include "Client.hpp"

bool Client::makeHeadMsg()
{
	/**
	 * TODO: status code (404 등) 먼저
	 */

	/**
	 * 404 아닐 경우
	 * Content-Language: ko
	 * Content-Location: <url>
	 * Content-Length: <length>
	 * Content-Type: text/plain
	 * TODO: Transfer-Encoding: chunked (나중에)
	 * TODO: retry-after를 exception 발생했을 시
	 * Last-Modified: <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
	 */
	return true;
}

void Client::makeGetMsg()
{
	// !효정님이 만들어주시는 _file_path 사용하기

	// TODO: body를 encoding 없이 raw로 push해주기
	std::ifstream file;
	struct stat info;
	const uint8_t* body;
	std::string line;

	// this->_file_path = "/Users/amin/Project/418-imateapot/";
	stat(this->_file_path.c_str(), &info);

	// autoindex
	if (S_ISDIR(info.st_mode) && this->_config_location->autoindex == true)
	{
		line = autoindex();
		body = reinterpret_cast<const uint8_t *>(line.c_str());
		// TODO: deque에 push_back이 제대로 들어가는지 잘 모르겠음.. 확인해야함..!
		this->_response.getBody().push_back(*body);
	}

	// read
	if (this->_config_location->autoindex == false)
	{
		file.open(this->_file_path.c_str(), O_RDONLY);
		while (!file.eof())
		{
			getline(file, line);
			body = reinterpret_cast<const uint8_t *>(line.c_str());
			// TODO: deque에 push_back이 제대로 들어가는지 잘 모르겠음.. 확인해야함..!
			this->_response.getBody().push_back(*body);
		}
		file.close();
	}
	// FIXME: 지우기!!!!!!!!!!!!!!!!!!!!!!!!1
	exit(1);
}

std::string Client::autoindex()
{
	DIR *dir;
	struct dirent *curr;
	std::string res;
	std::string url;

	url = this->_request.getHeaderValue("Host") + this->_request.getStartLine().path;
	dir = opendir(this->_file_path.c_str());
	res += "<html>\n<body>\n";
	res += "<h1>Directory listing</h1>\n";
	while ((curr = readdir(dir)) != NULL)
	{
		if (curr->d_name[0] != '.')
		{
			res += "<a href=\"" + url;
			//if (url != "/")
			//	res += "/";
			res += curr->d_name;
			res += "\">";
			res += curr->d_name;
			res += "</a><br>\n";
		}
	}
	closedir(dir);
	res += "</body>\n</html>\n";
	return (res);
}

void Client::makePutMsg()
{
	// TODO: 보류
}

void Client::makePostMsg()
{
	// TODO: 보류
}

/**
 * Client::Client
 * Client 생성자, 생성될 때 socket의 port번호를 받고 _status는 INITIALIZE로 초기화
 * @param  {Socket &socket} : class Socket
 */
Client::Client(Socket &socket):
	_port(socket.getPort()),
	_sock_status(INITIALIZE),
	_proc_status(PROC_INITIALIZE),
	_config_location(NULL)
{
	sockaddr	tmp;
	socklen_t	socksize = sizeof(sockaddr_in);

	if ((this->_fd = accept(socket.getFd(), &tmp, &socksize)) == -1)
		throw Client::SocketAcceptException();
}

/**
 * Client::parseStartLine
 * @brief  request의 start line을 파싱하여 Client의 멤버 변수에 저장한다.
 * @param  {std::string} line : request의 start line
 */
void Client::parseStartLine(const std::string &line)
{
	StartLineReq &start_line = this->_request.getStartLine();

	std::vector<std::string> split = ft_split(line, ' '); // method, path, protocol
	int method_num = methodToNum(split[0]);
	if (method_num < 0)
		throw Client::RequestFormatException();
	start_line.method = static_cast<e_method>(method_num);

	std::vector<std::string> split_path = ft_split(split[1], '/');
	if (split_path[0].find("http:") != std::string::npos) // absolute path로 요청되었을 경우의 전처리
	{
		this->_request.insertToHeader("Host", split_path[2]);
		split[1].erase(0, 7);							// remove "http://" from path
		split[1].erase(0, split_path[2].length());	// remove "host" from path
	}

	std::vector<std::string> split_query = ft_split(split[1], '?'); // query string 처리
	start_line.path = split_query[0];
	if (split_query.size() > 1)
		start_line.query_string = split_query[1];

	if (split[2].find("HTTP/") == std::string::npos) // if protocol is not HTTP
		throw Client::RequestFormatException();
	start_line.protocol = split[2];
}

/**
 * Client::parseHeader
 * response header 파싱
 * @param  {const std::string} line : header의 line
 */
void Client::parseHeader(const std::string &line)
{
	std::vector<std::string> headers = ft_split(line, ':');
	std::string value = line.substr(headers[0].size() + 1);
	ft_trim(value, " \t");
	this->_request.insertToHeader(headers[0], value);
}

/**
 * Client::setConfig
 * @brief  RECV_HEADER가 끝난 이후, Host와 Port를 기반으로 올바른 Config 인스턴스 연결
 * @param  {const ConfigGroup} group : Config 벡터를 가져올 ConfigGroup 인스턴스
 */
void Client::setConfig(ConfigGroup &group)
{
	std::string host = this->_request.getHeaderValue("Host");
	if (host.size() == 0)
		return;
	if (host.find(':') != std::string::npos)
		host.erase(host.find(':'));
	std::string path = this->_request.getStartLine().path;
	int length = group.getServerCnt();
	for (int i = 0; i < length; i++)
	{
		std::vector<Config> &server_config = group.getConfig(i);
		Config &defaultConfig = *(server_config.rbegin());
		if (this->_port != defaultConfig.port || host.compare(defaultConfig.server_name))
			continue;
		this->_config_location = &*(server_config.rbegin());
		for (int i = 0; i < server_config.size() - 1; i++)
		{
			std::string config_path = server_config[i].location_path;
			if (!path.compare(0, config_path.size(), config_path))
			{
				this->_config_location = &(server_config[i]);
				return;
			}
		}
		return;
	}
}

/**
 * Client::makeMsg
 *
 */
void Client::makeMsg()
{
	StartLineRes &res_start_line = this->_response.getStartLine();
	StartLineReq &req_start_line = this->_request.getStartLine();

	res_start_line.protocol = "HTTP/1.1";
	this->_response.insertToHeader("Server", "418-IAmATeapot");
	std::cout << res_start_line.protocol << std::endl;
	/**
	 * TODO: Date, Allow insert 구현
	 * Date: <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
	 * Allow: <http-methods>
	 */
	//switch (req_start_line.method)
	//{
	//case GET:
	this->makeGetMsg();
	//	break ;

	//case HEAD:
	//	this->makeHeadMsg();
	//	break ;

	//case PUT:
	//	break ;

	//case POST:
	//	break ;

	//default:
	//	break ;
	//}
}

/**
 * parseBuffer
 * buffer에 저장된 response를 각 부분(startline/header/body)에 맞게 함수 호출
 */
void Client::parseBuffer(char *buff, int len)
{
	size_t pos;

	this->_buffer.append(buff, len);
	/**
	 * TODO: recvBody 구현
	 * 		 Transfer-encoding 에 따른 body parsing 어떻게 할지
	 */
	if (this->_sock_status == INITIALIZE)
		this->_sock_status = RECV_START_LINE;
	while ((pos = this->_buffer.find('\n')) != std::string::npos)
	{
		std::string tmp = this->_buffer.substr(0, (this->_buffer[pos - 1] == '\r' ? pos - 1 : pos));
		if (this->_sock_status == RECV_START_LINE)
		{
			this->parseStartLine(tmp);
			this->_sock_status = RECV_HEADER;
		}
		else if (this->_sock_status == RECV_HEADER)
		{
			if (tmp.size() == 0)
			{
				this->_sock_status = RECV_BODY;
				this->_proc_status = CREATING;
			}
			else
				this->parseHeader(tmp);
		}
		else if (this->_sock_status == RECV_BODY)
		{
			// TODO: body 들어오는 경우 test 필요!!
		}
		this->_buffer.erase(0, pos + 1);
	}
}

int Client::getFd()
{
	return this->_fd;
}

e_proc_status Client::getProcStatus()
{
	return this->_proc_status;
}

const char *Client::SocketAcceptException::what() const throw()
{
	return ("SocketAcceptException: accept error!");
}

const char *Client::RequestFormatException::what() const throw()
{
	return ("RequestFormatException: Invalid Request!");
}
