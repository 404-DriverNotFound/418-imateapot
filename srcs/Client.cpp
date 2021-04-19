#include "Client.hpp"

/**
 * Client::Client
 * Client 생성자, 생성될 때 socket의 port번호를 받고 _status는 INITIALIZE로 초기화
 * @param  {Socket &socket} : class Socket
 */
Client::Client(Socket &socket): _port(socket.getPort()), _status(INITIALIZE)
{
	sockaddr	tmp;
	socklen_t	socksize = sizeof(sockaddr_in);

	if ((this->_fd = accept(socket.getFd(), &tmp, &socksize)) == -1)
		throw Client::SocketAcceptException();
}

/**
 * Client::recvStartLine
 * @brief  request의 start line을 파싱하여 Client의 멤버 변수에 저장한다.
 * @param  {std::string} line : request의 start line
 */
void Client::recvStartLine(const std::string &line)
{
	// ! rescStartLine -> parseStartLine 으로 함수명 변경 제안합니다.
	// ? this->_status = RECV_START_LINE; <- 넣을까 말까 하다가 뺐습니다.
	/**
	 * 이 함수는 수신이 완료되었다는 가정 하에 실행되므로 receive의 역할은
	 * 하지 않는 것 같습니다. status 배정 부분은 수신단에 있는 게 맞는 것 같네요.
	 * 그런 의미에서 함수명도 바꾸는 게 좋을 것 같습니다.
	 */
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
 * Client::recvHeader
 * response header 파싱
 * @param  {const std::string} line : header의 line
 */
void Client::recvHeader(const std::string &line)
{
	// TODO: \n 단위로 파싱을 해서 줄별로 넘겨받습니다.
	//       각 라인을 파싱해서 map에 넣어주시면 됩니다.
	std::cout << "HEADER : " << line << std::endl;
}

void Client::appendBuffer(char *buff, int len)
{
	this->_buffer.append(buff, len);
}
/**
 * parseBuffer
 * buffer에 저장된 response를 각 부분(startline/header/body)에 맞게 함수 호출
 */
void Client::parseBuffer()
{
	size_t pos;

	/**
	 * TODO: recvBody 구현
	 * 		 Transfer-encoding 에 따른 body parsing 어떻게 할지
	 * ! 선팍! recvHeader 구현해야됨!!!!
	 *
	 */
	if (this->_status == INITIALIZE)
		this->_status = RECV_START_LINE;
	while ((pos = this->_buffer.find('\n')) != std::string::npos)
	{
		std::string tmp = this->_buffer.substr(0, pos);
		if (this->_status == RECV_START_LINE)
		{
			recvStartLine(tmp);
			this->_status = RECV_HEADER;
		}
		else if (this->_status == RECV_HEADER)
		{
			if (tmp.size() == 0)
				this->_status = RECV_BODY;
			else
				recvHeader(tmp);
		}
		else if (this->_status == RECV_BODY)
		{
			// TODO: body 들어오는 경우 test 필요!!
		}
		this->_buffer.erase(0, tmp.length() + 1);
	}

	std::cout << "buffer >>" << this->_buffer << "<<" << std::endl;
}

int Client::getFd()
{
	return this->_fd;
}

const char *Client::SocketAcceptException::what() const throw()
{
	return ("SocketAcceptException: accept error!");
}

const char *Client::RequestFormatException::what() const throw()
{
	return ("RequestFormatException: Invalid Request!");
}
