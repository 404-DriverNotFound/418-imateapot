#include "Http.hpp"
#include <iostream>

/**
 * std::pair(key, value)를 만들어 http 헤더(map)에 insert해주는 함수.
 * 
 * @param  {std::string} key   : key
 * @param  {std::string} value : value
 */
void Http::insertToHeader(const std::string &key, const std::string &value)
{
	this->_headers.insert(std::make_pair(key, value));
}

/**
 * Http::getHeaderValue
 * 매개변수로 넣은 키값을 바탕으로 value를 반환해주는 함수
 * @param  {const std::string} key : 찾을 헤더의 key값
 * @return {std::string}           : 헤더가 있으면 value값, 없으면 빈 문자열 ("")
 */
std::string Http::getHeaderValue(const std::string &key)
{
	std::map<std::string, std::string>::iterator result = this->_headers.find(key);
	if (result == this->_headers.end())
		return "";
	return (*result).second;
}

std::string &Http::getBody()
{
	return (this->_body);
}

StartLineRes::StartLineRes() : status_code(200) {}

/**
 * start_line Getter for request
 *
 * @return {StartLineReq}  : request용 start line의 레퍼런스
 */
StartLineReq &HttpRequest::getStartLine()
{
	return (this->_start_line);
}

void HttpResponse::sendStartLine(int fd)
{
	std::string str;

	str += this->_start_line.protocol;
	str += " ";
	str += ft_itos(this->_start_line.status_code);
	str += " ";
	str += getStatusStr(this->_start_line.status_code);
	str += "\r\n";

	// FIXME: write, read는 모두 select를 거치도록 변경 
	write(fd, str.c_str(), str.length());
}

void HttpResponse::sendHeader(int fd)
{
	std::string str;

	std::map<std::string, std::string>::iterator ite = this->_headers.end();
	for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != ite; it++)
	{
		str += it->first;
		str += ": ";
		str += it->second;
		str += "\r\n";
	}
	str += "\r\n";
	write(fd, str.c_str(), str.length());
}

void HttpResponse::sendBody(int fd)
{
	write(fd, this->_body.c_str(), this->_body.length());
}

/**
 * start_line Getter for response
 *
 * @return {StartLineRes}  : response용 start line의 레퍼런스
 */
StartLineRes &HttpResponse::getStartLine()
{
	return (this->_start_line);
}

std::ostream &operator<<(std::ostream &o, StartLineReq &rhs)
{
	o << numToMethod(rhs.method) << " " << rhs.path << " " << rhs.protocol << std::endl;
	return o;
}
