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

    while (!config_file.eof())
    {
        std::string line;
        std::getline(config_file, line);
        if (isBlankLine(line))
            continue;
        if (line.compare("server"))
            throw ConfigGroup::ConfigFormatException();
        // TODO : server parsing 함수 추가
    }
    config_file.close();
}

ConfigGroup::~ConfigGroup() {}

/* --------- getter ---------*/
int ConfigGroup::getSeverCnt()
{
    return (_configs.length());
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