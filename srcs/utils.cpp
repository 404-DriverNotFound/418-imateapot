#include "webserv.hpp"

/**
 * ft_split
 * @brief  문자열을 잘라 vector로 반환해주는 함수
 * @param  {std::string} str           : 자를 문자열
 * @param  {char} delim                : 문자열 구분자 (한개만 가능)
 * @return {std::vector<std::string>}  : 잘라진 문자열 vector
 */
std::vector<std::string> ft_split(std::string str, char delim)
{
	std::vector<std::string> answer;
    int curr = 0, prev = 0;
    answer.clear();
   
    curr = str.find(delim);
    while (curr != std::string::npos)
    {
        answer.push_back(str.substr(prev, curr - prev));
        prev = curr + 1;
        curr = str.find(delim, prev);
    }
    answer.push_back(str.substr(prev, curr - prev));
    
    return answer;
}
