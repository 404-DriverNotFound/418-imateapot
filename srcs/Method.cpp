#include "webserv.hpp"

/**
 * method명을 입력하면 일치하는 숫자 값을 반환한다.
 * @param  {std::string} name : method명
 * @return {int}              : method 번호(0-4). 무효한 name이 들어올 시 -1(ERROR).
 */
int methodToNum(const std::string &name)
{
	if (!name.compare("GET"))
		return GET;
	else if (!name.compare("HEAD"))
		return HEAD;
	else if (!name.compare("PUT"))
		return PUT;
	else if (!name.compare("POST"))
		return POST;
	else if (!name.compare("DELETE"))
		return DELETE;
	else
		return ERROR;
}

/**
 * e_method형 숫자를 입력하면 일치하는 method명을 반환한다.
 * @param  {e_method} num : e_method (0-4)
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
	else if (num == DELETE)
		return "DELETE";
	return "ERROR";
}
/**
 *
 * @param  {bool*} methods : config에 있는 method[n]의 bool값
 * @return {std::string}   : method list
 */
std::string makeMethodList(bool *methods)
{
	std::string	list;
	for (int i = 0; i <= POST; i++)
		if (methods[i])
		{
			list.append(numToMethod((e_method)i));
			list.append(", ");
		}
	if (!list.empty())
		list.erase(list.length() - 2);
	return (list);
}
