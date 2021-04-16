#include "Client.hpp"
#include "utils.hpp"

/**
 * request의 start line을 파싱하여 Client의 멤버 변수에 저장한다.
 * 
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
	if (!split_query[1].empty())
		start_line.query_string = split_query[1];

	if (split[2].find("HTTP/") == std::string::npos) // if protocol is not HTTP
		throw Client::RequestFormatException();
	start_line.protocol = split[2];
}

const char *Client::RequestFormatException::what() const throw()
{
	return ("RequestFormatException: Invalid Request!");
}
