#include "Client.hpp"

/**
 * Client::makeFilePath
 * HTTP request로 얻은 path에 해당하는 file path를 구성.
 */
void Client::makeFilePath()
{
	std::string &path = this->_request.getStartLine().path;
	Config &config = *this->_config_location;

	this->_file_path.clear();
	this->_file_path = config.root;

	if (!config.location_path.empty())
	{
		std::string dir_path = path.substr(config.location_path.length());
		this->_file_path.append(dir_path);
	}
	else if (path.length() > 1)
		this->_file_path.append(path);
	/**
	 * std::cout << "path: " << this->_file_path << std::endl;
	 * 일단 테스트용으로 남겨둬봅니다.
	*/
}

/**
 * Client::checkFilePath
 * makeFilePath에서 구성한 file path가 유효한지 검사한 후 status code 부여.
 */
void Client::checkFilePath()
{
	Config &config = *this->_config_location;
	struct stat	path_stat;

	if (stat(this->_file_path.c_str(), &path_stat)) // if stat() failed
	{
		switch (errno)
		{
		case EACCES:
		case EFAULT:
		case ENOENT:
		case ENOTDIR:
			throw 404;
		default:
			throw 503;
		}
		return ;
	}

	if (!FT_S_ISDIR(path_stat.st_mode)) // if _file_path is a file
	{
		std::string last_modified = getHttpTimeFormat(path_stat.st_mtime);
		std::cout << last_modified << std::endl;
		this->_response.insertToHeader("Last-modified", last_modified);
		return ;
	}
	// if _file_path is a dir
	std::string root = config.root;

	if (config.autoindex) // if autoindex is on
		return ;

	// if autoindex is off
	if (this->_file_path != root) // if not in root
		throw 404;

	// if in root
	int fd;
	if ((fd = open(config.index.c_str(), O_RDONLY)) == -1)
		throw 404;
	close(fd);
	this->_file_path = config.index;
}

/**
 * Client::makeContentLocation 
 * file_path로부터 Content-Location을 구성.
 * @return {std::string}  : 만들어진 content-location
 */
std::string Client::makeContentLocation()
{
	Config &config = *this->_config_location;
	std::string host = this->_request.getHeaderValue("Host");

	std::string content_location = this->_file_path;

	if (content_location.find(config.root) != std::string::npos)
	{
		content_location.erase(0, config.root.length());
		if (!config.location_path.empty())
			content_location.insert(0, config.location_path);
	}
	else
		content_location.erase(0, config.server_root.length());
	return (content_location);
}

/**
 * Client::makeHeadMsg
 * HEAD 메소드로 들어온 HTTP request에 대한 response 메시지를 구성.
 */
void Client::makeHeadMsg()
{
	StartLineRes &start_line = this->_response.getStartLine();

	this->makeFilePath();
	this->checkFilePath();
	std::string content_location = this->makeContentLocation();

	std::cout << "code: " << (unsigned int)start_line.status_code << std::endl;
	std::cout << "path: " << this->_file_path << std::endl;
	std::cout << "contentLocation: " << content_location << std::endl;
	exit(1); // FIXME: 지울 것

	this->_response.insertToHeader("Content-Language", "ko");
	this->_response.insertToHeader("Content-Location", content_location);
	this->_response.insertToHeader("Content-Type", "text/plain");
	this->_response.insertToHeader("Transfer-Encoding", "chunked");
}

void Client::makeGetMsg()
{
	this->makeHeadMsg();
	// TODO: body를 encoding 없이 raw로 push해주기
	std::ifstream file;
	struct stat info;
	const uint8_t* body;
	std::string line;

	stat(this->_file_path.c_str(), &info);

	// autoindex
	if (S_ISDIR(info.st_mode) && this->_config_location->autoindex == true)
	{
		line = this->autoindex();
		body = reinterpret_cast<const uint8_t *>(line.c_str());
		// TODO: deque에 push_back이 제대로 들어가는지 잘 모르겠음.. 확인해야함..!
		this->_response.getBody().push_back(*body);
	}

	// read
	if (this->_config_location->autoindex == false)
	{
		file.open(this->_file_path.c_str());
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
			res += "<a href=\"http://" + url;
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
	_file_path(),
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
		throw 400;
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
		throw 400;
	start_line.protocol = split[2];
}

/**
 * Client::parseHeader
 * request header 파싱
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
		return ;

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
		for (unsigned long i = 0; i < server_config.size() - 1; i++)
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

void Client::makeBasicHeader()
{
	StartLineRes &start_line = this->_response.getStartLine();

	start_line.protocol = "HTTP/1.1";
	this->_response.insertToHeader("Server", "418-IAmATeapot");
	this->_response.insertToHeader("Date", getCurrentTime());
}

/**
 * Client::makeMsg
 *
 */
void Client::makeMsg()
{
	StartLineReq &start_line = this->_request.getStartLine();

	std::cout << start_line << std::endl;

	if (!this->_config_location->auth.empty())
	{
		std::string auth_value = this->_request.getHeaderValue("Authorization");
		if (auth_value.empty())
			throw 401;
		std::vector<std::string> temp = ft_split(auth_value, ' ');
		if (temp.size() != 2 || temp[0].compare("Basic") || temp[1].compare(this->_config_location->auth))
			throw 403;
	}

	if (!(this->_config_location->method[start_line.method]))
		throw 405;

	this->makeBasicHeader();

	switch (start_line.method)
	{
	case HEAD:
		this->makeHeadMsg();
		break ;

	case GET:
		this->makeGetMsg();
		break ;

	case PUT:
		break ;

	case POST:
		break ;

	default:
		break ;
	}
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

/**
 * Client::makeStatus
 * response에 status code를 부여.
 * @param  {int} status : 부여하고자 하는 status code.
 */
void Client::makeErrorStatus(uint16_t status)
{
	StartLineRes &start_line = this->_response.getStartLine();
	Config &config = *this->_config_location;
	int fd;

	start_line.status_code = status;

	/**
	 * TODO: 408(Request Timeout)
	 */
	switch (status)
	{
	case 400:
	case 403:
	case 404:
	case 413:
	case 431:
	case 503:
	case 505:
		break ;
		// 위 status code에는 헤더를 추가할 필요가 없음.

	case 413:
	case 503:
		this->_response.insertToHeader("Retry-After", "120");
		break ;
	
	case 401:
		this->_response.insertToHeader("WWW-Authenticate", "Basic");
		break ;

	case 405:
		this->_response.insertToHeader("Allow", makeMethodList(this->_config_location->method));
		break ;

	case 418:
		std::cerr << "☕ TEA-POT! ☕" << std::endl;
		break;

	default:
		break ;
	}

	if ((fd = open(config.error_page.c_str(), O_RDONLY)) == -1)
	{
		this->_file_path = "";
		return ;
	}
	close(fd);
	this->_file_path = config.error_page;
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
