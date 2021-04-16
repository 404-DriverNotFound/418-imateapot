#include "webserv.hpp"

/**
 * std::pair(key, value)를 만들어 http 헤더(multimap)에 insert해주는 함수.
 * 
 * @param  {std::string} key   : key
 * @param  {std::string} value : value
 */
void Http::insertToHeader(const std::string &key, const std::string &value)
{
	_headers.insert(std::make_pair(key, value));
}

/**
 * start_line Getter for request
 * 
 * @return {StartLineReq}  : request용 start line의 레퍼런스
 */
StartLineReq &HttpRequest::getStartLine()
{
	return (this->_start_line);
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
