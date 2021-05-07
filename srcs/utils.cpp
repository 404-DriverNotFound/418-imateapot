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
	size_t curr = 0;
	int prev = 0;
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

/**
 * ft_atoi
 * @brief  cstdlib의 atoi의 대체함수입니다.
 * @param  {std::string} str : 10진수 숫자의 문자열
 * @return {int}             : 변환된 숫자
 */
int	ft_atoi(const std::string &str)
{
	long long result = 0;
	int minus_flag = 1, index = 0;

	if (!str.empty() && (str[index] == '+' || str[index] == '-') && static_cast<int>(str.length()) > index)
	{
		if (str[index] == '-')
			minus_flag = -1;
		index++;
	}
	while ('0' <= str[index] && str[index] <= '9' && static_cast<int>(str.length()) > index)
	{
		result *= 10;
		result += (str[index] - '0');
		index++;
	}
	result *= minus_flag;
	return static_cast<int>(result);
}

std::string ft_itos(int num)
{
	std::string str;
	long long	nbr = (long long)num;

	if (num == 0)
		return ("0");
	if (num < 0)
		nbr *= -1;
	while (nbr != 0)
	{
		str = static_cast<char>((nbr % 10) + 48) + str;
		nbr /= 10;
	}
	if (num < 0)
		str = "-" + str;
	return (str);
}

std::string ft_ultohex(unsigned long num)
{
	std::string str;
	unsigned long tmp;

	if (num == 0)
		return ("0");
	while (num != 0)
	{
		tmp = num % 16;
		tmp = (tmp >= 10 ? (tmp + 'A' - 10) : (tmp + '0'));
		str = static_cast<char>(tmp) + str;
		num /= 16;
	}
	return (str);
}

/**
 * ft_atoi
 * @brief  cstdlib의 strtol의 대체함수입니다. 그중 unsigned 16진수에 한정지어 처리합니다.
 * @param  {std::string} str : 16진수 숫자의 문자열
 * @return {int}             : 변환된 숫자
 */
unsigned long	ft_uhextol(const std::string &str)
{
	unsigned long	result = 0;
	int				index = 0;

	while ((('0' <= str[index] && str[index] <= '9') ||
			('A' <= str[index] && str[index] <= 'F') ||
			('a' <= str[index] && str[index] <= 'f')) && static_cast<int>(str.length()) > index )
	{
		result *= 16;
		if (str[index] <= '9')
			result += (str[index] - '0');
		else if (str[index] <= 'F')
			result += (str[index] - 'A' + 10);
		else
			result += (str[index] - 'a' + 10);
		index++;
	}
	return result;
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

bool isFilePath(const std::string &path)
{
	struct stat info;
	
	if (stat(path.c_str(), &info) == 0)
	{
		if (S_ISREG(info.st_mode))
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

bool isDirPath(const std::string &path)
{
	struct stat info;

	if (stat(path.c_str(), &info) == 0)
	{
		if (S_ISDIR(info.st_mode))
			return 1;
		else 
			return 0;
	}
	else
		return 0;
}

std::string getStatusStr(uint16_t code)
{
    switch (code) {
        case 200:
            return ("OK");
        case 201:
            return ("Created");
        case 202:
            return ("Accepted");
		case 204:
			return ("No Content");
        case 400:
            return ("Bad Request");
        case 401:
			return ("Unauthorized");
		case 403:
            return ("Forbidden");
        case 404:
            return ("Not Found");
		case 405:
			return ("Method Not Allowed");
		case 413:
			return ("Payload Too Large");
		case 418:
			return ("I'm a teapot");
        case 500:
            return ("Internal Server Error");
		case 503:
			return ("Service Unavailable");
		case 505:
			return ("HTTP Version Not Supported");
    }
    return ("");
}

std::string ft_inet_ntoa(unsigned int addr)
{
	unsigned int n = addr;

	std::string res = std::to_string(n % 256) + ".";
	res += std::to_string((n / 256) % 256) + ".";
	res += std::to_string((n / 256 / 256) % 256) + ".";
	res += std::to_string(n / 256 / 256 / 256);

	return (res);
}

uint16_t ft_htons(uint16_t port)
{
	uint16_t res = (((((unsigned short)(port) & 0xFF)) << 8) | (((unsigned short)(port) & 0xFF00) >> 8));

	return (res);
}

u_int32_t ft_htonl(unsigned long int addr)
{
	u_int32_t res = (((((unsigned long)(addr) & 0xFF)) << 24) | \
					((((unsigned long)(addr) & 0xFF00)) << 8) | \
					((((unsigned long)(addr) & 0xFF0000)) >> 8) | \
					((((unsigned long)(addr) & 0xFF000000)) >> 24));
	
	return (res);
}