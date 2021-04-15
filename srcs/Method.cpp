#include "Method.hpp"

/**
 * method명을 입력하면 일치하는 숫자 값을 반환한다.
 * @param  {std::string} name : method명
 * @return {int}              : method 번호(0-6). 무효한 name이 들어올 시 -1.
 */
int methodToNum(const std::string &name)
{
	if (name.compare("GET"))
		return GET;
	else if (name.compare("HEAD"))
		return HEAD;
	else if (name.compare("PUT"))
		return PUT;
	else if (name.compare("POST"))
		return POST;
	else if (name.compare("TRACE"))
		return TRACE;
	else if (name.compare("DELETE"))
		return DELETE;
	else
		return -1;
}

/**
 * e_method형 숫자를 입력하면 일치하는 method명을 반환한다.
 * @param  {e_method} num : e_method (0-6)
 * @return {std::string}  : method명
 */
std::string numToMethod(const e_method num)
{
	if (num == GET)
		return "GET";
	else if (num == HEAD)
		return "HEAD";
	else if (num == PUT)
		return "PUT";
	else if (num == POST)
		return "POST";
	else if (num == TRACE)
		return "TRACE";
	else if (num == DELETE)
		return "DELETE";
}
