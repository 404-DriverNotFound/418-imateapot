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

/**
 * isBlankLine
 * 가져온 문자열에 공백이나 탭으로만 이루어져있는지 확인하는 함수
 * @param  {std::string} line : 확인할 문자열
 * @return {bool}             : 공백이나 탭으로만 이루어져있으면 참
 */
bool isBlankLine(const std::string &line)
{
	for (size_t i = 0; i < line.size(); i++)
	{
        if (line[i] != ' ' && line[i] != '\t')
            return false;
    }
    return true;
}

/**
 * ft_trim
 * @brief  문자열의 앞뒤에서 해당하는 문자들을 자른다.
 * @param  {std::string} str : 대상 문자열
 * @param  {std::string} cut : 자를 문자 모음
 */
void ft_trim(std::string &str, const std::string cut)
{
	std::size_t found = str.find_last_not_of(cut);
	if (found != std::string::npos)
		str.erase(found + 1);
	else
		str.clear();

	found = str.find_first_not_of(cut);
	if (found != std::string::npos)
		str.erase(0, found);
	else
		str.clear();
}

std::string getHTTPTimeFormat(time_t time)
{
	char s[150];
    struct tm *tm_time = std::gmtime(&time);

	strftime(s, sizeof(s), "%a, %d %b %Y %T GMT", tm_time);
    return (s);
}

std::string getCurrentTime()
{
    struct timeval time;
    gettimeofday(&time, NULL);
	return getHTTPTimeFormat(time.tv_sec);
}
