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
	else
		this->_file_path.append(path);

	if (this->_file_path[this->_file_path.length() - 1] == '/')
		this->_file_path.erase(this->_file_path.length() - 1);
}

/**
 * Client::checkFilePath
 * makeFilePath에서 구성한 file path가 유효한지 검사한 후 status code 부여.
 */
void Client::checkFilePath()
{
	Config &config = *this->_config_location;
	std::string index_path;
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
	}

	if (isFilePath(this->_file_path)) // if _file_path is a file
	{
		std::string last_modified = getHTTPTimeFormat(path_stat.st_mtime);
		this->_response.insertToHeader("Last-modified", last_modified);
		this->_response.insertToHeader("Content-Length", ft_itos(path_stat.st_size));
		return ;
	}
	// if _file_path is a dir
	std::string root = config.root;

	if (config.autoindex) // if autoindex is on
		return ;

	// if autoindex is off
	// if (this->_file_path != root) // if not in root
	// 	throw 404;

	// if in root
	index_path = _file_path + "/" + config.index;

	if (stat(index_path.c_str(), &path_stat)) // if stat() failed
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
	}
	if (isDirPath(index_path))
		throw 404;
	
	this->_file_path = index_path;
	this->_response.insertToHeader("Content-Length", ft_itos(path_stat.st_size));
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

std::string Client::makeAutoindex()
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

/**
 * Client::makeHeadMsg
 * HEAD 메소드로 들어온 HTTP request에 대한 response 메시지를 구성.
 */
void Client::makeHeadMsg()
{
	this->checkFilePath();
	std::string content_location = this->makeContentLocation();

	this->_response.insertToHeader("Content-Location", content_location);
	this->_response.insertToHeader("Content-Language", "ko");
	this->_response.insertToHeader("Content-Type", "text/plain");
}

void Client::makeGetMsg()
{
	std::ifstream file;
	struct stat info;
	std::string line;

	if (this->isCGIRequest())
	{
		this->execCGI();
		return ;
	}
	
	this->makeHeadMsg();

	stat(this->_file_path.c_str(), &info);

	// autoindex
	if (isDirPath(_file_path) && this->_config_location->autoindex == true)
	{
		line = this->makeAutoindex();
		this->_response.getBody() += line;
	}

	// read
	if (this->_config_location->autoindex == false)
	{
		this->_read_fd = open(this->_file_path.c_str(), O_RDONLY);
		if (this->_read_fd == -1)
			throw 503;
	}
}

void Client::makePutMsg()
{
	int		fd;

	if (isFilePath(_file_path))
	{
		fd = open(_file_path.c_str(), O_WRONLY | O_TRUNC, 0666);
		if (fd == -1)
			throw 403;
		this->_response.getStartLine().status_code = 204;
	}
	else
	{
		fd = open(_file_path.c_str(), O_CREAT | O_WRONLY, 0666);
		if (fd == -1)
			throw 503;
		this->_response.getStartLine().status_code = 201;
		this->_response.insertToHeader("Content-Length", "0");
	}
	this->_write_fd = fd;
	std::string content_location = this->makeContentLocation();

	this->_response.insertToHeader("Location", content_location);
	this->_response.insertToHeader("Content-Location", content_location);
}

void Client::makePostMsg()
{
	if (_response.getBody().size() == 0)
		makeGetMsg();
	else if (this->isCGIRequest())
		this->execCGI();
}

bool Client::isCGIRequest()
{
	if (_config_location->cgi_extension.empty())
		return false;

	std::vector<std::string> split = ft_split(_request.getStartLine().path, '.');

	if (split[split.size() - 1] != this->_config_location->cgi_extension)
		return false;
	return true;
}

char **Client::setEnv()
{
	char								**env = 0;
	std::map<std::string, std::string>	map_env;
	size_t								pos;

	if (!(this->_request.getHeaderValue("Authorization").empty()))
	{
		pos = this->_request.getHeaderValue("Authorization").find(" ");
		map_env["AUTH_TYPE"] = this->_request.getHeaderValue("Authorization").substr(0, pos);
		map_env["REMOTE_USER"] = this->_request.getHeaderValue("Authorization").substr(pos + 1);
		map_env["REMOTE_IDENT"] = this->_request.getHeaderValue("Authorization").substr(pos + 1);
	}

	map_env["CONTENT_LENGTH"] = ft_itos(this->_request.getBody().size());

	if (!(this->_request.getHeaderValue("Content-Type").empty()))
		map_env["CONTENT_TYPE"] = this->_request.getHeaderValue("Content-Type");

	map_env["GATEWAY_INTERFACE"] = "CGI/1.1";
	map_env["PATH_INFO"] = this->_request.getStartLine().path;
	map_env["PATH_TRANSLATED"] = this->_file_path;

	map_env["QUERY_STRING"];
	if (!(this->_request.getStartLine().query_string.empty()))
		map_env["QUERY_STRING"] = this->_request.getStartLine().query_string;

	map_env["REMOTE_ADDR"] = this->_socket->getIp();
	map_env["REQUEST_METHOD"] = numToMethod(this->_request.getStartLine().method);

	map_env["REQUEST_URI"] = this->_request.getStartLine().path ;
	if (!this->_request.getStartLine().query_string.empty())
		map_env["REQUEST_URI"] += "?" + this->_request.getStartLine().query_string;
	map_env["SCRIPT_NAME"] = this->_config_location->cgi_path;
	map_env["SERVER_NAME"] = _config_location->server_name;
	map_env["SERVER_PORT"] = ft_itos(_config_location->port);
	map_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	map_env["SERVER_SOFTWARE"] = "418-IAmATeapot";

	std::map<std::string, std::string>::iterator hit = this->_request.getHeaders().begin();
	std::map<std::string, std::string>::iterator hite = this->_request.getHeaders().end();
	for (; hit != hite; hit++)
	{
		map_env["HTTP_" + hit->first] = hit->second;
	}

	if (!(env = (char **)malloc(sizeof(char *) * (map_env.size() + 1))))
		throw 500;
	std::map<std::string, std::string>::iterator it = map_env.begin();
	int i = 0;
	while (it != map_env.end())
	{
		env[i] = strdup((it->first + "=" + it->second).c_str());
		++i;
		++it;
	}
	env[i] = NULL;
	return (env);
}

void Client::execCGI()
{
	std::string tmp_name = ".TMP_FILE" + ft_itos(getFd());
	int		ret, in_fd[2], tmp_fd;
	char	*args[3];
	char	**env = this->setEnv();
	std::string temp_string;

	this->_buffer.clear();

	args[0] = strdup(this->_config_location->cgi_path.c_str());
	args[1] = strdup(this->_file_path.c_str());
	args[2] = NULL;

	if ((pipe(in_fd) == -1) || ((this->_pid = fork()) == -1))
		throw 500;

	if ((tmp_fd = open(tmp_name.c_str(), O_CREAT | O_TRUNC | O_RDWR, 0666)) == -1)
		throw 500;

	this->_sock_status = PROC_CGI_HEADER;

	if (this->_pid == 0)
	{
		close(in_fd[1]);
		dup2(in_fd[0], 0);
		dup2(tmp_fd, 1);
		ret = execve(args[0], args, env);
		free(args[0]);
		free(args[1]);
		close(in_fd[0]);
		exit(ret);
	}
	free(args[0]);
	free(args[1]);
	close(in_fd[0]);
	this->_write_fd = in_fd[1];
	this->_read_fd = tmp_fd;
	this->freeEnv(env);
}

void Client::freeEnv(char **env)
{
	int i = 0;

	while (env[i])
		free(env[i++]);
	free(env);
}

void Client::parseCGIBuffer()
{
	std::string &body = this->_response.getBody();
	std::string tmp;
	size_t pos;
	size_t index;

	while ((pos = body.find('\n')) != std::string::npos)
	{
		index = (pos ? (body[pos - 1] == '\r' ? pos - 1 : pos) : pos);
		tmp = body.substr(0, index);
		std::vector<std::string> split = ft_split(tmp, ':');
		body.erase(0, pos + 1);
		if (tmp.empty())
		{
			this->_sock_status = PROC_CGI_BODY;
			break ;
		}
		ft_trim(split[1], " \t");
		if (split[0] == "Status")
			this->_response.getStartLine().status_code = ft_atoi(split[1]);
		else
			this->_response.insertToHeader(split[0], split[1]);
	}
}

/**
 * Client::Client
 * Client 생성자, 생성될 때 socket의 port번호를 받고 _status는 INITIALIZE로 초기화
 * @param  {Socket &socket} : class Socket
 */
Client::Client(Socket &socket, int fd):	
	_pid(-1),
	_port(socket.getPort()),
	_fd(fd),
	_read_fd(-1),
	_write_fd(-1),
	_content_length_left(EMPTY_CONTENT_LENGTH),
	_chunked_length(CHUNKED_READY),
	_sock_status(INITIALIZE),
	_config_location(NULL),
	_socket(&socket),
	_is_read_finished(false)
{
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
 * Client::setClientResReady
 * @brief  RECV_HEADER가 끝난 이후, Host와 Port를 기반으로 올바른 Config 인스턴스 연결
 * @param  {const ConfigGroup} group : Config 벡터를 가져올 ConfigGroup 인스턴스
 */
void Client::setClientResReady(ConfigGroup &group)
{
	std::string host = this->_request.getHeaderValue("Host");

	if (host.size() == 0)
		throw 400;

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
			if (!path.compare(config_path) ||
				!path.compare(0, config_path.size() + 1, config_path + '/'))
			{
				this->_config_location = &(server_config[i]);
				return;
			}
		}
		return;
	}
}
/**
 * Client::parseBody
 * @brief  request body 파트를 파싱하는 함수
 * @return {int}  : body의 수신 종료 상태를 알려주는 상태 코드
 */
int	Client::parseBody()
{
	std::string tmp;
	size_t pos;

	if (this->_content_length_left != EMPTY_CONTENT_LENGTH)
	{
		if (static_cast<int>(this->_buffer.length()) > this->_content_length_left)
		{
			this->_request.getBody() += this->_buffer.substr(0, this->_content_length_left);
			this->_buffer.erase(0, this->_content_length_left);
			this->_content_length_left = 0;
		}
		else
		{
			this->_content_length_left -= this->_buffer.length();
			this->_request.getBody() += this->_buffer;
			this->_buffer.clear();
		}
		if (this->_content_length_left <= 0)
			return PARSE_BODY_END;
	}
	else
	{
		while (!this->_buffer.empty() && (pos = this->_buffer.find('\n')) != std::string::npos)
		{
			if (this->_chunked_length == CHUNKED_READY)
			{
				tmp = this->_buffer.substr(0, (this->_buffer[pos - 1] == '\r' ? pos - 1 : pos));
				this->_chunked_length = static_cast<int>(ft_uhextol(tmp));
				this->_buffer.erase(0, pos + 1);
			}
			else
			{
				// chunked 형식의 끝
				if (this->_chunked_length == 0)
				{
					this->_buffer.erase(0, pos + 1);
					return PARSE_BODY_END;
				}
				tmp = this->_buffer;
				pos = tmp.length();
				if (static_cast<int>(pos) > this->_chunked_length)
				{
					tmp.erase(this->_chunked_length);
					pos = this->_chunked_length;
				}
				this->_request.getBody() += tmp;
				this->_chunked_length -= pos;

				if (this->_chunked_length <= 0)
				{
					this->_chunked_length = CHUNKED_READY;
					this->_buffer.erase(0, pos + 2);
				}
				else
					this->_buffer.erase(0, pos);
			}
		}
	}
	
	return PARSE_BODY_LEFT;
}

void Client::makeBasicHeader()
{
	StartLineRes &start_line = this->_response.getStartLine();

	start_line.protocol = "HTTP/1.1";
	this->_response.insertToHeader("Server", "418-IAmATeapot");
	this->_response.insertToHeader("Date", getCurrentTime());
	this->_response.insertToHeader("Content-Type", "text/plain");
}

/**
 * Client::makeMsg
 *
 */
void Client::makeMsg()
{
	StartLineReq &start_line = this->_request.getStartLine();
	this->makeFilePath();

	std::cout << start_line << std::endl;

	this->_sock_status = MAKE_MSG;

	if (!this->_config_location->auth.empty())
	{
		std::string auth_value = this->_request.getHeaderValue("Authorization");
		if (auth_value.empty())
			throw 401;
		std::vector<std::string> temp = ft_split(auth_value, ' ');
		if (temp.size() != 2 || temp[0].compare("Basic") || temp[1].compare(this->_config_location->auth))
			throw 403;
	}

	this->makeBasicHeader();

	if (!(this->_config_location->method[start_line.method]))
	{
		switch (start_line.method)
		{
			case GET:
			case POST:
				if (this->isCGIRequest())
				{
					this->execCGI();
					return;
				}
			default:
				throw 405;
		}
	}

	switch (start_line.method)
	{
	case HEAD:
		this->makeHeadMsg();
		break ;

	case GET:
		this->makeGetMsg();
		break ;

	case PUT:
		this->makePutMsg();
		break ;

	case POST:
		this->makePostMsg();
		break ;

	default:
		break ;
	}
}

void Client::sendMsg()
{
	if (this->_sock_status == SEND_HEADER)
	{
		std::cout << "========WRITE=======\n";
		this->_response.sendHeader(this->getFd());
		this->_sock_status = (this->_response.getBody().empty() ? SEND_DONE : SEND_BODY);
	}
	else
	{
		if (this->_response.sendBody(this->getFd()) == SEND_BODY_DONE)
			this->_sock_status = SEND_DONE;
	}
	if (this->_sock_status == SEND_DONE)
		std::cout << "=====================\n";
}

/**
 * parseBuffer
 * buffer에 저장된 response를 각 부분(startline/header/body)에 맞게 함수 호출
 */
void Client::parseBuffer(char *buff, int len, ConfigGroup &configs)
{
	std::string tmp;
	size_t idx;
	size_t pos;

	// 이미 모두 다 받았을때 입력된 내용 버리기
	if (this->_sock_status >= MAKE_READY)
		return ;

	this->_buffer.append(buff, len);
	if (this->_sock_status == INITIALIZE)
		this->_sock_status = RECV_START_LINE;
	while ((pos = this->_buffer.find('\n')) != std::string::npos && this->_sock_status != RECV_BODY)
	{
		idx = (pos ? (this->_buffer[pos - 1] == '\r' ? pos - 1 : pos) : pos);
		tmp = this->_buffer.substr(0, idx);
		if (this->_sock_status == RECV_START_LINE)
		{
			this->parseStartLine(tmp);
			this->_sock_status = RECV_HEADER;
		}
		else if (this->_sock_status == RECV_HEADER)
		{
			if (tmp.size() == 0)
			{
				this->setClientResReady(configs);
				this->_sock_status = RECV_BODY;
				std::string content_length_str = this->_request.getHeaderValue("Content-Length");
				if (!content_length_str.empty())
				{
					if ((this->_content_length_left = ft_atoi(content_length_str)) == 0)
					{
						this->_sock_status = MAKE_READY;
						this->_buffer.erase(0, pos + 1);
						return;
					}
				}
				else if (this->_request.getHeaderValue("Transfer-Encoding").compare("chunked"))
				{
					this->_sock_status = MAKE_READY;
					this->_buffer.erase(0, pos + 1);
					return;
				}
			}
			else
				this->parseHeader(tmp);
		}
		this->_buffer.erase(0, pos + 1);
	}
	if (this->_sock_status == RECV_BODY)
	{
		if (this->parseBody() == PARSE_BODY_END) // 받는 과정 끝났을 때!
		{
			if (this->_request.getBody().length() > this->_config_location->body_length)
				throw 413;
			this->_sock_status = MAKE_READY;
		}
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
	std::ifstream file;
	std::string line;
	int fd;

	start_line.status_code = status;

	switch (status)
	{
	case 400:
	case 403:
	case 404:
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

	if (!this->isConfigSet())
	{
		this->_response.getBody() += ft_itos(status);
		this->_response.getBody() += " ";
		this->_response.getBody() += getStatusStr(status);
		this->_response.insertToHeader("Content-Length", ft_itos(this->_response.getBody().size()));
		this->_sock_status = SEND_HEADER;
		return ;
	}

	std::string error_path;
	if (isDirPath(_file_path))
		error_path = this->_file_path + "/" + config.error_page;
	else
	{
		size_t pos = this->_file_path.find_last_of('/');
		if (pos == std::string::npos)
			error_path = this->_file_path + config.error_page;
		else
			error_path = this->_file_path.substr(0, pos + 1) + config.error_page;
	}

	if ((fd = open(error_path.c_str(), O_RDONLY)) == -1)
	{
		this->_response.getBody() += ft_itos(status);
		this->_response.getBody() += " ";
		this->_response.getBody() += getStatusStr(status);
		this->_response.insertToHeader("Content-Length", ft_itos(this->_response.getBody().size()));
		this->_sock_status = SEND_HEADER;
		return ;
	}
	close(fd);

	this->_file_path = error_path;

	this->_read_fd = open(this->_file_path.c_str(), O_RDONLY);
	if (this->_read_fd == -1)
		throw 503;
}

void Client::readData(fd_set &fd_read_set)
{
	int len, status;
	char buf[BUF_SIZE];

	if (this->_sock_status == PROC_CGI_HEADER && this->_pid != -1)
	{
		if (!waitpid(this->_pid, &status, WNOHANG))
			return ;
		if (WIFEXITED(status) == 0)
			throw 500;
		this->_pid = -1;
	}

	len = read(this->_read_fd, buf, BUF_SIZE - 1);
	
	if (len < 0)
		throw 503;
	
	if (len == 0)
	{
		close(this->_read_fd);
		FT_FD_CLR(this->_read_fd, &fd_read_set);
		this->_read_fd = -1;
		this->_response.insertToHeader("Content-Length", ft_itos(this->_response.getBody().length()));
		this->_response.insertToHeader("Content-Language", "ko");
		if (this->_sock_status == PROC_CGI_BODY)
		{
			std::string name = ".TMP_FILE" + ft_itos(getFd());
			unlink(name.c_str());
		}
		this->_sock_status = SEND_HEADER;
		return ;
	}
	
	buf[len] = '\0';
	this->_response.getBody() += buf;

	if (this->_sock_status == PROC_CGI_HEADER)
		this->parseCGIBuffer();
}

void Client::writeData(fd_set &fd_write_set)
{
	int ret;
	
	ret = write(this->_write_fd, this->_request.getBody().c_str(), this->_request.getBody().length());
	if (ret == -1)
		throw 503;
	FT_FD_CLR(this->_write_fd, &(fd_write_set));
	close(this->_write_fd);
	this->_write_fd = -1;
	if (this->_sock_status == MAKE_MSG)
		this->_sock_status = SEND_HEADER;
}

bool Client::isConfigSet()
{
	return (this->_config_location != NULL);
}

void Client::reset(fd_set &fd_read_set, fd_set &fd_write_set)
{
	this->_pid = -1;
	if (this->_read_fd != -1)
	{
		FT_FD_CLR(this->_read_fd, &(fd_read_set));
		this->_read_fd = -1;
	}
	if (this->_write_fd != -1)
	{
		FT_FD_CLR(this->_write_fd, &(fd_write_set));
		this->_write_fd = -1;
	}
	this->_content_length_left = EMPTY_CONTENT_LENGTH;
	this->_chunked_length = CHUNKED_READY;
	this->_sock_status = INITIALIZE;
	this->_request = HttpRequest();
	this->_response = HttpResponse();
	this->_file_path = "";
	this->_config_location = NULL;
}

int Client::getFd()
{
	return this->_fd;
}

int Client::getReadFd()
{
	return this->_read_fd;
}

int Client::getWriteFd()
{
	return this->_write_fd;
}

e_sock_status Client::getSockStatus()
{
	return this->_sock_status;
}

bool Client::getIsReadFinished()
{
	return this->_is_read_finished;
}

void Client::setIsReadFinished(bool is_read_finished)
{
	this->_is_read_finished = is_read_finished;
}

void Client::setReadFd(int fd)
{
	this->_read_fd = fd;
}

void Client::setWriteFd(int fd)
{
	this->_write_fd = fd;
}

const char *Client::SocketAcceptException::what() const throw()
{
	return ("SocketAcceptException: accept error!");
}
