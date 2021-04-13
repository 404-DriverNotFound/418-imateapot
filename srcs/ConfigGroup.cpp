#include "ConfigGroup.hpp"

/**
 * ConfigGroup 
 * config file 전체를 파싱하는 생성자
 * @param  {std::string} path        : config file 경로 
 * @param  {uint32_t} max_connection : 서버에서 받을 수 있는 최대 연결 수 (기본값 20)
 */
ConfigGroup::ConfigGroup(const std::string &path, uint32_t max_connection = 20): _max_connection(max_connection)
{
    std::ifstream config_file(path);

    if (!config_file.is_open())
        throw ConfigGroup::NoConfigFileException();

	std::string line;
    std::getline(config_file, line);

    while (!config_file.eof())
    {
        if (isBlankLine(line))
            continue;
        if (line.compare("server"))
            throw ConfigGroup::ConfigFormatException();
        this->parseServer(config_file, line);
    }
    config_file.close();
}

ConfigGroup::~ConfigGroup() {}

void ConfigGroup::parseServer(std::ifstream config_file, std::string &line)
{
	bool is_location_start = false;
	std::vector<Config> server_vector;

	std::getline(config_file, line);

	while (!config_file.eof())
    {
        if (isBlankLine(line))
            continue;
        if (line[0] != '\t')
			break;
		if (line[1] == '\t')
			throw ConfigGroup::ConfigFormatException();
		std::vector<std::string> split = ft_split(line.substr(1), ' ');
		if (!split[0].compare("location"))
		{
			is_location_start = true;
			// TODO : Location 파싱하기
			continue;
		}
		if (is_location_start ||
			(split[0].compare("method") && split.size() > 2))
			throw ConfigGroup::ConfigFormatException();
		
		std::getline(config_file, line);
    }
}

/* --------- getter ---------*/
int ConfigGroup::getSeverCnt()
{
    return (_configs.size());
}

uint32_t ConfigGroup::getMaxConnection()
{
    return (_max_connection);
}

std::vector<Config> &ConfigGroup::getConfig(int index)
{
    return (_configs[index]);
}

const char *ConfigGroup::NoConfigFileException::what() const throw()
{
    return ("NoConfigFileException: there is no file!");
}

const char *ConfigGroup::ConfigFormatException::what() const throw()
{
    return ("ConfigFormatException: Invalid format!");
}

/**
 * isBlankLine
 * 가져온 문자열에 공백이나 탭으로만 이루어져있는지 확인하는 함수
 * @param  {std::string} line : 확인할 문자열
 * @return {bool}             : 공백이나 탭으로만 이루어져있으면 참
 */
bool isBlankLine(const std::string &line)
{
    for (int i = 0; i < line.size(); i++)
    {
        if (line[i] != ' ' && line[i] != '\t')
            return false;
    }
    return true;
}