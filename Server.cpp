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
	listen(_socketServer.fd, 5);
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
//	struct stat buf;

//	std::cout << "incheckuser" << std::endl;
	it = _users.begin();
	while (it != _users.end())
	{
//		std::cout << "inzewhile" << std::endl;
		std::cout << it->second.getSocket().revents << std::endl;
		if ( (it->second.getSocket().revents & 1) == POLLIN) 
		{
//			std::cout << "POLLIN" << std::endl;
		//	fstat(it->second.getSocket().fd, &buf);
		//	std::cout << "bufftruc" <<  buf.st_size << std::endl;
		//	if (buf.st_size > 0)
		//	{
			int rett = recv(it->second.getSocket().fd, buffer, 512, MSG_DONTWAIT);
			if (rett == 0)	
				perror("riennalir :");
			if (rett == -1)	
				perror("recv :");
			else if (rett > 0)
			{
				std::cout << "buffer = "<< buffer << std::endl;
				it->second.receivedmsg.push_back(buffer);
			}
			bzero(buffer, 512);
			--(*ret);
	//		}
		}
		if ( (it->second.getSocket().revents & 4) == POLLOUT) 
		{
			if (it->second.tosendmsg.empty())
			{
				if (send(it->second.getSocket().fd, RPL_PING, strlen(RPL_PING), MSG_DONTWAIT) == -1)
					perror("send :");
				--(*ret);
				return ;
			}
//			std::cout << "POLLout = to send : " << it->second.tosendmsg.front() << std::endl;	
		//	std::cout << sizeof(char *) << " ; " << sizeof(void*)<<  " ; " << sizeof(RPL_WELCOME) << " : "<< sizeof(&(it->second.tosendmsg.front())) << std::endl;
			if (send(it->second.getSocket().fd, it->second.tosendmsg.front(), strlen(it->second.tosendmsg.front()), MSG_DONTWAIT) == -1)
				perror("send :");
			else
				it->second.tosendmsg.pop_front();
			--(*ret);
		}
		++it;
	}
}

void	Server::_pollfunction(void) 
{
	int		ret;
	char	buffer[512];
//	struct stat buf;
		
	//std::cout << "indc = " << _nbConn - _nbUsers << " flag= " << _pollTab[0].revents << "fd = "<< _pollTab[0].fd  << " nbconnect = " << _nbConn << std::endl;
	ret = poll(_pollTab, _nbConn, 7000);
		for (int i = 0;  i < _nbConn; ++i)
	//			std::cout << "i = " << i << " event = " << _pollTab[i].events << "revent = " << _pollTab[i].revents << std::endl;
	if (ret == 0)
		std::cout << "Timeout\n";
	else if (ret == -1)
		perror("poll :");
	else
	{
		if (_nbUsers)
			_checkUser(&ret);
		if (_nbConn - _nbUsers > 1) // && _pollTab[_nbConn - 1].revents == POLLIN)
		{
			if (recv(_pollTab[_nbConn - 1].fd, buffer, 512, MSG_DONTWAIT) > -1)
			{
			//	fstat(_pollTab[_nbConn - 1].fd, &buf);
			//	std::cout << "bufftruc" <<  buf.st_size << std::endl;
				if (_initClient(_pollTab[_nbConn - 1], buffer))
					send(_pollTab[_nbConn].fd, ERR_NICKNAMEINUSE,
						sizeof(ERR_NICKNAMEINUSE), 0);//trouve qqchos de mieux stp
			}
			else
				perror("recevage");
		}
		if (_pollTab[0].revents == POLLIN)
		{
			_initSocket();
			--ret;
		}
		if (ret > 0)
		{
	//		std::cout << "reste du ret de poll == " << ret << std::endl;
	//		for (int i = 0;  i < _nbConn; ++i)
	//			std::cout << "i = " << i << " event = " << _pollTab[i].events << "revent = " << _pollTab[i].revents << std::endl;
		}
	}
}

void	Server::_initSocket(void)
{
	struct pollfd	socket;

//	std::cout << "i'm in init socket" << std::endl;
	socket.fd = accept(_socketServer.fd, (struct sockaddr* )&_addrClient,
			&_clientSize);
	if (socket.fd > 0 && fcntl(socket.fd, F_SETFL, O_NONBLOCK) > -1)
	{
		socket.events = POLLIN | POLLOUT;
		socket.revents = 0;
		_pollTab[_nbConn] = socket;
//		std::cout << "scket is in emplacement " << _nbConn << std::endl;

		++_nbConn;
	}
	else
		perror("new socket");
}

int		Server::_initClient(struct pollfd socket, char *buffer)
{
	User						user(socket);
	std::pair<std::map<std::string, User>::iterator, bool>	tmp;

//	std::cout << "i'm in init client" << std::endl;
	user.parseUser(buffer);
	tmp = _users.insert( std::pair<std::string, User>(user.getNickname(), user) );
//	std::cout << "nick = " << user.getNickname() << " user= " << user.getUsername()
//		<< " ip= " << user.getIp() << " realname= " << user.getRealname() << std::endl;

	if (tmp.second == true)
	{
		++_nbUsers;
		_users[user.getNickname()].tosendmsg.push_back(RPL_WELCOME);
		_users[user.getNickname()].tosendmsg.push_back(RPL_YOURHOST);
		_users[user.getNickname()].tosendmsg.push_back(RPL_CREATED);
		_users[user.getNickname()].tosendmsg.push_back(RPL_MYINFO);
	//	_users[user.getNickname()]._socket.events = POLLIN | POLLOUT;

		
//		std::cout << "Client with fd number " << socket.fd << " was created. It got " << _users[user.getNickname()].tosendmsg.size() << "messages tp send" << std::endl;
	}
	else
	{
//		std::cout << "mefaitej" << std::endl;
		--_nbConn;
		close(socket.fd);
		return (1);
	}
	return (0);
}

int		Server::start(void)
{
	while (1)
	{
		_pollfunction();
//		std::cout << "Users registered = " << _nbUsers << std::endl;
	//	sleep(2);
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


