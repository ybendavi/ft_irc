#include "Server.hpp"

Server::Server(int port)
{
	_pollTab = new struct pollfd[MAX_CONN];
	_clientSize = sizeof(_addrClient);
	_nbUsers = 0;
	_nbConn = 0;

	_socketServer.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socketServer.fd < 0 || fcntl(_socketServer.fd, F_SETFL, O_NONBLOCK) < 0)
	{
		perror("creation server");
		exit(1);
	}
	++_nbConn;
	_socketServer.events = POLLIN;
	_socketServer.revents = 0;

	_addrServer.sin_family = AF_INET;
	_addrServer.sin_addr.s_addr = htonl(INADDR_ANY);
	_addrServer.sin_port = htons(port);
	std::cout << bind(_socketServer.fd, (const struct sockaddr *)&_addrServer, sizeof(_addrServer)) << " bind" << std::endl;
	_pollTab[0] = _socketServer;
}

Server::~Server(void)
{
	close(_socketServer.fd);
}

void	Server::_checkUser(int *ret)
{
	char	buffer[512];
	std::map<std::string, User>::iterator	it;

	it = _users.begin();
	while (it != _users.end())
	{
		switch (it->second.getSocket().revents) 
		{
			case POLLIN :
				if (recv(it->second.getSocket().fd, buffer, 512, MSG_DONTWAIT) > 0)
					it->second.receivedmsg.push_back(buffer);
				else		
					perror("recv :");
				bzero(buffer, 512);
				--(*ret);
				break ;

			case POLLOUT :
				if (send(it->second.getSocket().fd,
							&(it->second.tosendmsg.front()), sizeof(char *),
							MSG_DONTWAIT) == -1)
					perror("send :");
				else
					it->second.tosendmsg.pop_front();
				--(*ret);
				break ;

			default :
				break ;
		}
		++it;
	}
}

void	Server::_pollfunction(bool b) 
{
	int		ret;
	char	buffer[512];
	
	std::cout << "i'm in" << std::endl;
	
	ret = poll(_pollTab, _nbConn, 7000);
	if (ret == 0)
		std::cout << "Timeout\n";
	else if (ret == -1)
		perror("poll :");
	else
	{
		if (b)
		{
			listen(_pollTab[0].fd, 1);
			return ;
		}
		if (_nbUsers)
			_checkUser(&ret);
		else if (_nbConn - _nbUsers > 1 && _pollTab[_nbConn].revents == POLLIN)
		{
			if (recv(_pollTab[_nbConn].fd, buffer, 512, MSG_DONTWAIT) > 0)
			{
				if (_initClient(_pollTab[_nbConn], buffer))
					send(_pollTab[_nbConn].fd, ERR_NICKNAMEINUSE,
					sizeof(ERR_NICKNAMEINUSE), 0);//trouve qqchos de mieux stp
			}
			else
				perror("recevage");
		}
		else if (_pollTab[0].revents == POLLIN)
		{
			_initSocket();
			--ret;
		}
		std::cout << "reste du ret de poll == " << ret << std::endl;
	}

}

void	Server::_initSocket(void)
{
	struct pollfd	socket;

	socket.fd = accept(_socketServer.fd, (struct sockaddr* )&_addrClient,
			&_clientSize);
	if (socket.fd < 0 || fcntl(socket.fd, F_SETFL, O_NONBLOCK) < -1)
	{
		socket.events = POLLIN;
		socket.revents = 0;
		_pollTab[_nbConn] = socket;
		++_nbConn;
	}
	else
		perror("new socket");
}

int		Server::_initClient(struct pollfd socket, char *buffer)
{
	User						user(socket);
	std::pair<std::map<std::string, User>::iterator, bool>	tmp;

	user.parseUser(buffer);
	tmp = _users.insert( std::pair<std::string, User>(user.getNickname(), user) );
	std::cout << "nick = " << user.getNickname() << " user= " << user.getUsername()
		<< " ip= " << user.getIp() << " realname= " << user.getRealname() << std::endl;

	if (tmp.second == true)
	{
		++_nbUsers;
		_users[user.getNickname()].tosendmsg.push_back(RPL_WELCOME);
		_users[user.getNickname()].tosendmsg.push_back(RPL_YOURHOST);
		_users[user.getNickname()].tosendmsg.push_back(RPL_CREATED);
		_users[user.getNickname()].tosendmsg.push_back(RPL_MYINFO);

		std::cout << "Client with fd number " << socket.fd << " was created." << std::endl;
	}
	else
	{
		--_nbConn;
		close(socket.fd);
		return (1);
	}
	return (0);
}

int		Server::start(void)
{
	_pollfunction(1);
	while (1)
	{
		_pollfunction(0);
		std::cout << "Users registered = " << _nbUsers << std::endl;
	}
}

/*
int		Server::handleClient(void)
		{
			size_t	i;
			char	buffer[512];
	
			i = 0;
				
			while (i < _nbUsers)
			{		
				if (recv(socket_clients[i], buffer, 512, MSG_DONTWAIT) > 0)
				{
					std::cout << "i = " << i << std::endl;
					messages.push_back(std::string(buffer));
					std::cout << buffer << std::endl;
					bzero(buffer, 512);
					send(socket_clients[i], RPL_WELCOME, sizeof(RPL_WELCOME), 0);
			}	
				if (recv(socket_clients[i], buffer, 512, MSG_DONTWAIT) > 0)
				{
					messages.push_back(std::string(buffer));
					std::cout << buffer << std::endl;
					bzero(buffer, 512);
				}

				send(socket_clients[i], RPL_YOURHOST, sizeof(RPL_YOURHOST), 0);
				
				if (recv(socket_clients[i], buffer, 512, MSG_DONTWAIT) > 0)
				{
					messages.push_back(std::string(buffer));
					std::cout << buffer << std::endl;
					bzero(buffer, 512);
				}

				send(socket_clients[i], RPL_CREATED, sizeof(RPL_CREATED), 0);
				
				if (recv(socket_clients[i], buffer, 512, MSG_DONTWAIT) > 0)
				{
					messages.push_back(std::string(buffer));
					std::cout << buffer << std::endl;
					bzero(buffer, 512);
				}

				send(socket_clients[i], RPL_MYINFO, sizeof(RPL_MYINFO), 0);
					
				if (recv(socket_clients[i], buffer, 512, MSG_DONTWAIT) > 0)
				{
					messages.push_back(std::string(buffer));
					std::cout << buffer << std::endl;
					bzero(buffer, 512);
				}
				i++;
			}
			return (0);
		}*/


