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
		uint16_t port = this->_configs.getConfig(i).rbegin()->port;
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
	fd_set temp_fd_read, temp_fd_write;
	int state;

	while (1)
	{
		memcpy(&temp_fd_read, &(this->_fd_read), sizeof(fd_set));
		memcpy(&temp_fd_write, &(this->_fd_write), sizeof(fd_set));

		state = select(FT_FD_SETSIZE, &temp_fd_read, &temp_fd_write, NULL, NULL);
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
					try
					{
						this->_clients.push_back(Client(this->_socks[i]));
					}
					catch(const std::exception& e)
					{
						std::cerr << e.what() << '\n';
						break ;
					}
					Client &created = *(this->_clients.rbegin());
					FT_FD_SET(created.getFd(), &(this->_fd_read));
					FT_FD_SET(created.getFd(), &(this->_fd_write));
				}
			std::map<int, int> error_info;
			for (unsigned long i = 0; i < this->_clients.size(); i++)
			{
				// TODO: client 상태에 따라 read하지 않고 continue;
				if (FT_FD_ISSET(this->_clients[i].getFd(), &(temp_fd_read)))
				{
					try
					{
						this->readRequest(this->_clients[i]);
					}
					catch(int error_status)
					{
						error_info.insert(std::make_pair<int, int>(i, error_status));
					}
				}
			}
			this->handleHttpError(error_info, true);

			for (unsigned long i = 0; i < this->_clients.size(); i++)
			{
				if (this->_clients[i].getProcStatus() == PROC_INITIALIZE)
					continue ;
				if (FT_FD_ISSET(this->_clients[i].getFd(), &(temp_fd_write)))
				{
					try
					{
						this->handleResponse(this->_clients[i]);
					}
					catch (int error_status)
					{
						error_info.insert(std::make_pair<int, int>(i, error_status));
					}
				}
			}
			this->handleHttpError(error_info, false);
		}
	}
}

/**
 * Webserver::readRequest
 * @brief  select를 통해 들어온 특정 소켓의 데이터를 처리하는 함수
 * @param  {Socket} sock : select를 통해 데이터가 들어온 것을 확인한 소켓
 */
void Webserver::readRequest(Client &client)
{
	char	buff[READ_BUFFER];
	int		len;

	len = read(client.getFd(), buff, READ_BUFFER);
	if (len < 0)
		throw 503;
	if (len == 0)
	{
		// TODO: 정상 종료되었을때 response 보내고 close하기!!
		std::cout << "END!!!!!\n";
		return ;
	}
	client.parseBuffer(buff, len);
}

void Webserver::handleResponse(Client &client)
{
	if (client.getProcStatus() == CREATING)
	{
		client.setConfig(this->_configs);
		client.makeMsg();
	}
		// create
	else if (client.getProcStatus() == SENDING)
		;
		// write
}

void Webserver::handleHttpError(std::map<int, int>& error_info, bool is_request)
{
	std::map<int, int>::reverse_iterator rite = error_info.rend();

	for (std::map<int, int>::reverse_iterator rit = error_info.rbegin(); rit != rite; rit++) 
	{
		std::vector<Client>::iterator client = this->_clients.begin() + rit->first;

		if (is_request)
			client->makeBasicHeader();

		client->makeErrorStatus(rit->second);
		// TODO: sendMsg 해주고 종료해야 함
		close(client->getFd());
		FT_FD_CLR(client->getFd(), &(this->_fd_read));
		FT_FD_CLR(client->getFd(), &(this->_fd_write));
		this->_clients.erase(client);
	}
	error_info.clear();
}

const char *Webserver::SelectException::what() const throw()
{
	return "SelectException: fail to run select!";
}

const char *Webserver::SocketReadException::what() const throw()
{
	return "SocketReadException: fail to read socket!";
}
