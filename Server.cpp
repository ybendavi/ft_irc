#include "Server.hpp"

Server::Server(void)
{
	_pollTab = new struct pollfd[MAX_CONN];
	_clientSize = sizeof(_addrClient);
	_nbUsers = 0;
	_nbConn = 0;
	_ret = 0;
}

Server::~Server(void)
{
	while (_nbConn > 0)
	{
		--_nbConn;
		if (_pollTab[_nbConn].fd != -1)
			close(_pollTab[_nbConn].fd);
	}
	delete [] _pollTab;
}

int	Server::init(int port)
{
	_pollTab[0].fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_pollTab[0].fd < 0)
		return (-1);
	if (fcntl(_pollTab[0].fd, F_SETFL, O_NONBLOCK) < 0)
		return (-2);
	++_nbConn;
	_pollTab[0].events = POLLIN;
	_pollTab[0].revents = 0;

	_addrServer.sin_family = AF_INET;
	_addrServer.sin_addr.s_addr = htonl(INADDR_ANY);
	_addrServer.sin_port = htons(port);
	if ( bind(_pollTab[0].fd, (const struct sockaddr *)&_addrServer,
			sizeof(_addrServer)) )
		return (-3);
	if (listen(_pollTab[0].fd, 5))
		return (-4);
	return (0);
}

void	Server::_checkUser(int *ret)
{
	char	buffer[512];
	std::map<std::string, User>::iterator	it;

	it = _users.begin();
	while (it != _users.end())
	{
	//	std::cout << it->second.getSocket().revents << std::endl;
		if ( (it->second.getSocket().revents & 1) == POLLIN) 
		{
			if ( recv(it->second.getSocket().fd, buffer, 512, MSG_DONTWAIT) == -1 )
				_ret = -5;
			else
				it->second.receivedmsg.push_back(buffer);
			bzero(buffer, 512);
			--(*ret);
		}
		if (_ret)
			return ;
		if ( (it->second.getSocket().revents & 4) == POLLOUT) 
		{
			if (it->second.tosendmsg.empty()) //virer ce if si on a pas besoin de ping ou le --
			{
				if (send(it->second.getSocket().fd, RPL_PING, strlen(RPL_PING), MSG_DONTWAIT) == -1)
					_ret = -6;
				--(*ret);
				return ;
			}
//			std::cout << "POLLout = to send : " << it->second.tosendmsg.front() << std::endl;	
		//	std::cout << sizeof(char *) << " ; " << sizeof(void*)<<  " ; " << sizeof(RPL_WELCOME) << " : "<< sizeof(&(it->second.tosendmsg.front())) << std::endl;
			if (send(it->second.getSocket().fd, it->second.tosendmsg.front(), strlen(it->second.tosendmsg.front()), MSG_DONTWAIT) == -1)
				_ret = -6;
			else
				it->second.tosendmsg.pop_front();
			--(*ret);
		}
		if (_ret)
			return ;
		++it;
	}
}

void	Server::_pollfunction(void) 
{
	int		ret;
		
//	std::cout << "indc = " << _nbConn - _nbUsers << " flag= " << _pollTab[0].revents << "fd = "<< _pollTab[0].fd  << " nbconnect = " << _nbConn << std::endl;
	ret = poll(_pollTab, _nbConn, 7000);
//		for (int i = 0;  i < _nbConn; ++i)
//				std::cout << "i = " << i << " event = " << _pollTab[i].events << "revent = " << _pollTab[i].revents << std::endl;
	if (ret == 0)
		std::cout << "Timeout\n";
	else if (ret == -1)
		perror("poll :"); //au moment ou tu ecriras ta gestion d'user qui part faut gerr ca
	else
	{
		if (_nbUsers)
			_checkUser(&ret);
		if (_ret)
			return ;
		while (_nbConn - _nbUsers > 1)
		{	
			if (_initClient(_nbConn - _nbUsers - 1) < 0)
				return ;
		}
		if (_pollTab[0].revents == POLLIN)
		{
			_ret = _initSocket();
			if (_ret)
				return ;
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

int		Server::_initSocket(void)
{
//	std::cout << "nb = " << _nbConn << std::endl;
	_pollTab[_nbConn].fd  = accept(_pollTab[0].fd, (struct sockaddr* )&_addrClient,
			&_clientSize);
	if (_pollTab[_nbConn].fd  == -1)
		return (-1);
	if (fcntl(_pollTab[_nbConn].fd , F_SETFL, O_NONBLOCK) == -1)
		return (-2);
	_pollTab[_nbConn].events = POLLIN | POLLOUT;
	_pollTab[_nbConn].revents = 0;
	++_nbConn;
	return (0);
}

int		Server::_initClient(int index)
{
	char			buffer[512];
	std::string		nick;

	sleep(1);
//	if ( !((_pollTab[index].revents & 1) == POLLIN) )
//		return (0);
	std::cout << index << " = index"  << std::endl;
	if (recv(_pollTab[index].fd, buffer, 512, MSG_DONTWAIT) == -1)
	{
		--_nbConn;
		close(_pollTab[index].fd);
		_ret = -5;
		return (0);
	}
	std::cout << "goto nick\n";
	nick = findNick(std::string(buffer));
	std::cout << "nick = " << nick << " buffer = " << buffer << std::endl; 
	if ( _users.find(nick) == _users.end() )
	{
		User		user( _pollTab[index] );

		std::cout << "goto user\n";
		user.parseUser( buffer );
		_users.insert( std::pair<std::string, User>(nick, user) );
		++_nbUsers;
		_users[nick].tosendmsg.push_back(RPL_WELCOME);
		_users[nick].tosendmsg.push_back(RPL_YOURHOST);
		_users[nick].tosendmsg.push_back(RPL_CREATED);
		_users[nick].tosendmsg.push_back(RPL_MYINFO);
	}
	else
	{
		std::cout << "goto err\n";
		--_nbConn;
		send(_pollTab[_nbConn].fd, ERR_NICKNAMEINUSE,
			strlen(ERR_NICKNAMEINUSE), 0);
		close(_pollTab[index].fd);
		return (1);
	}
	return (0);
}

int		Server::start(void)
{
	while (1)
	{
		_pollfunction();
		if (_ret)
			return (_ret);
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


