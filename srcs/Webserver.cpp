#include "webserv.hpp"
/**
 * Webserver 
 * @brief  전체 웹서버 인스턴스를 만드는 생성자
 * @param  {const std::string} path  : 설정 파일의 경로
 * @param  {uint32_t} max_connection : 최대 접속 가능수
 */
Webserver::Webserver(const std::string &path, uint32_t max_connection) : _configs(ConfigGroup(path, max_connection))
{
	std::vector<u_int16_t> server_ports;

	for (int i = 0; i < this->_configs.getServerCnt(); i++)
	{
		this->_servers.push_back(Server(this->_configs.getConfig(i)));
		uint16_t port = this->_servers[i].getPort();
		if (std::find(server_ports.begin(), server_ports.end(), port) == server_ports.end())
			server_ports.push_back(port);
	}
	
	FT_FD_ZERO(&(this->_fd_read));
	FT_FD_ZERO(&(this->_fd_write));
	for (size_t i = 0; i < server_ports.size(); i++)
	{
		this->_socks.push_back(Socket(server_ports[i], max_connection));
		FT_FD_SET(this->_socks[i].getFd(),&(this->_fd_read));
	}
}

/**
 * Webserver::startServer 
 * @brief  Webserver 내 Server를 바탕으로 서버를 시작하는 함수. 이 함수에서 프로젝트 유일하게 select가 작동됨.
 */
void Webserver::startServer()
{
	fd_set temp_fd_read;
	int state;

	while (1)
	{
		memcpy(&temp_fd_read, &(this->_fd_read), sizeof(fd_set));
		
		/** 
		* TODO: select의 max_fd 값은 고정인지 유동인지 
		*/
		state = select(FT_FD_SETSIZE, &temp_fd_read, NULL, NULL, NULL);
		switch (state)
		{
		case -1:
			throw Webserver::SelectException();
		case 0:
			continue;
		default:
			for (size_t i = 0; i < this->_socks.size(); i++)
				if (FT_FD_ISSET(this->_socks[i].getFd(), &(temp_fd_read)))
				{
					this->_clients.push_back(Client(this->_socks[i]));
					Client &created = *(this->_clients.rbegin());
					FT_FD_SET(created.getFd(), &(this->_fd_read));
					FT_FD_SET(created.getFd(), &(this->_fd_write));
				}
			bool is_connection_finish = true;
			while (is_connection_finish)
			{
				is_connection_finish = false;
				std::vector<Client>::iterator ite = this->_clients.end();
				for (std::vector<Client>::iterator it = this->_clients.begin(); it < ite; it++)
					if (FT_FD_ISSET(it->getFd(), &(temp_fd_read)))
					{
						try
						{
							is_connection_finish |= readRequest(it);
							if (is_connection_finish)
								break;
						}
						catch(const std::exception& e)
						{
							std::cerr << e.what() << '\n';
						}
					}
				}
			
		}
	}
}

/**
 * Webserver::readRequest
 * @brief  select를 통해 들어온 특정 소켓의 데이터를 처리하는 함수
 * @param  {Socket} sock : select를 통해 데이터가 들어온 것을 확인한 소켓
 */
bool Webserver::readRequest(std::vector<Client>::iterator client_it)
{
	char	buff[READ_BUFFER];
	int		len;

	len = read(client_it->getFd(), buff, READ_BUFFER);
	if (len <= 0)
	{
		close(client_it->getFd());
		FT_FD_CLR(client_it->getFd(), &(this->_fd_read));
		FT_FD_CLR(client_it->getFd(), &(this->_fd_write));
		this->_clients.erase(client_it);
		if (len < 0)
			throw Webserver::SocketReadException();
		std::cout << "END!!!!!\n";
		return true;
	}
	client_it->appendBuffer(buff, len);
	client_it->parseBuffer();
	return false;
}

const char *Webserver::SelectException::what() const throw()
{
	return "SelectException: fail to run select!";
}

const char *Webserver::SocketReadException::what() const throw()
{
	return "SocketReadException: fail to read socket!";
}
