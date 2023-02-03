#include "Server.hpp"
#include "signal.hpp"

extern volatile sig_atomic_t loop;

Server::Server(void) : _ret(0), _clientSize(sizeof(struct sockaddr) ) , _nbUsers(0), _nbSock(0), _on(1), _off(0)
{ 
	bzero( _addrInfo, sizeof(struct sockaddr));
}

Server::~Server(void)
{
	while (_nbSock-- > 0)
	{
		//--_nbSock; add decrement in condition
		if (_pollTab[_nbSock].fd != -1)
			close(_pollTab[_nbSock].fd);
	}
}

int	Server::init(int port)
{
	if (!(port >= 1 && port <= 65535))
		return (-7);

	_pollTab[0].fd = socket(PF_INET6, SOCK_STREAM, 0);
	if (_pollTab[0].fd < 0)
		return (-1);

	if (fcntl(_pollTab[0].fd, F_SETFL, O_NONBLOCK) < 0)
		return (-2);

	_pollTab[0].events = POLLIN;
	_pollTab[0].revents = 0;
	++_nbSock;

	if ( setsockopt(_pollTab[0].fd, SOL_SOCKET, SO_REUSEADDR, &_on, sizeof(_on)) == -1 )
		return (-8);
	if ( setsockopt(_pollTab[0].fd, IPPROTO_IPV6, IPV6_V6ONLY, &_off, sizeof(_off)) == -1 )
		return (-9);

	memset(&_addrServer, 0, sizeof(_addrServer));
	_addrServer.sin6_family = AF_INET6;
	_addrServer.sin6_addr = in6addr_any;
	_addrServer.sin6_port = htons(port);
	if ( bind(_pollTab[0].fd, (sockaddr *)&_addrServer, sizeof(_addrServer)) )
		return (-3);

	if (listen(_pollTab[0].fd, MAX_USER)) //what macro should we use in listen call ?
		return (-4);

	return (0);
}

void	Server::_disconnectClient(iterator leaving, iterator replace)
{
	std::cout << "Client " << leaving->first << " disconnected" << std::endl;

	close(leaving->second.getSocket()->fd);
	leaving->second.getSocket()->fd = replace->second.getSocket()->fd;
	_users.erase(leaving);
	--_nbUsers;
}

void	Server::_checkUser(int *ret)
{
	char	buffer[512];
	int		status;
	std::map<std::string, User>::iterator	it;

	it = _users.begin();
	while (it != _users.end())
	{
	//	std::cout << it->second.getSocket().revents << std::endl;
		if ( (it->second.getSocket()->revents & (POLLNVAL|POLLERR|POLLHUP) ))
			std::cout << "client disconnected" << std::endl;
		if ( (it->second.getSocket()->revents & 1) == POLLIN) 
		{
			status = recv(it->second.getSocket()->fd, buffer, 512, MSG_DONTWAIT); 
			
			if (status <= 0)
			{
				if (status == -1)
					_ret = -5;
				if (status == 0)
					_disconnectClient(it, --_users.end());
			}
			else
			{
				it->second.receivedmsg.push_back(Message(buffer));
				it->second.execute();
			//	std::cout << "incoming = " << buffer << std::endl;
			}
			bzero(buffer, 512);
			--(*ret);
		}
		if (_ret)
			return ;
		if ( (it->second.getSocket()->revents & 4) == POLLOUT) 
		{
			if (it->second.tosendmsg.empty()) //virer ce if si on a pas besoin de ping ou le --
				return ;
//			std::cout << "POLLout = to send : " << it->second.tosendmsg.front() << std::endl;	
		//	std::cout << sizeof(char *) << " ; " << sizeof(void*)<<  " ; " << sizeof(RPL_WELCOME) << " : "<< sizeof(&(it->second.tosendmsg.front())) << std::endl;
		//	std::cout << "mdg = " << it->second.tosendmsg.front().getToSend().c_str();
			if (send(it->second.getSocket()->fd, it->second.tosendmsg.front().getToSend().c_str(),
						strlen(it->second.tosendmsg.front().getToSend().c_str()), MSG_DONTWAIT) == -1)
				_ret = -6;
			else
				it->second.tosendmsg.pop_front();
			--(*ret);
		}
		if (_ret)
			return ;
		++it;
	//	std::cout << "un pachage\n";
	}
}

void	Server::_pollfunction(void) 
{
	int		ret; //pour l instant on le garde pis si il sert pas dans le else on vire l'autre variable et on use lui pr init cli
	int		cli;
		
//	for (int i = 0;  i < _nbSock; i++)
//			std::cout << "before : i = " << i << " event = " << _pollTab[i].events << "revent = " << _pollTab[i].revents << std::endl;
	ret = poll(_pollTab, _nbSock, 7000);
//	for (int i = 0;  i < _nbSock; i++)
//			std::cout << "after : i = " << i << " event = " << _pollTab[i].events << "revent = " << _pollTab[i].revents << std::endl;
	if (ret == 0)
		std::cout << "Timeout\n";
	else if (ret == -1)
		perror("poll :"); //au moment ou tu ecriras ta gestion d'user qui part faut gerr cap
	else if ( (_pollTab[0].events & (POLLNVAL|POLLERR|POLLHUP) ))
		std::cout << "buggybugg" << std::endl;
	else
	{
		if (_nbUsers)
			_checkUser(&ret);
		if (_ret)
			return ;
		cli = _nbSock - _nbUsers;
		while (cli > 1)
		{
			if (_initClient(_nbUsers + 1) < 0)
					return ;
			--cli;
		}
		if (_pollTab[0].revents == POLLIN)
		{
			_ret = _initSocket();
			if (_ret)
				return ;
		}
		if (ret > 0)
		{
	//		std::cout << "reste du ret de poll == " << ret << std::endl;
	//		for (int i = 0;  i < _nbSock; ++i)
	//			std::cout << "i = " << i << " event = " << _pollTab[i].events << "revent = " << _pollTab[i].revents << std::endl;
		}
	}
}

int		Server::_initSocket(void)
{
//	std::cout << "init sockt\n";
	_pollTab[_nbSock].fd  = accept(_pollTab[0].fd, (struct sockaddr* )&_addrInfo[_nbSock],
			&_clientSize);
	if (_pollTab[_nbSock].fd  == -1)
		return (-1);
	if (fcntl(_pollTab[_nbSock].fd , F_SETFL, O_NONBLOCK) == -1)
		return (-2);
	_pollTab[_nbSock].events = POLLIN | POLLOUT;
	_pollTab[_nbSock].revents = 0;
	++_nbSock;
	return (0);
}

int		Server::_initClient(int index)
{
	char			buffer[512];
	std::string		nick;

	if ( !((_pollTab[index].revents & 1) == POLLIN) ) //resolve your fucking parser its ugly and glitchy
		return (0);
	if (recv(_pollTab[index].fd, buffer, 512, MSG_DONTWAIT) == -1)
	{
		--_nbSock;
		close(_pollTab[index].fd);
		_ret = -5;
		return (0);
	}
//	std::cout <<"buffer = " << buffer << std::endl;
	nick = findNick(std::string(buffer));
//	std::cout <<"nick =" <<nick << std::endl;
	
	if ( _users.find(nick) == _users.end() )
	{
		User		user( &(_pollTab[index]), &(_addrInfo[index]) );
    
	//	std::cout << "goto user\n";
		user.parseUser( buffer );
		_users.insert( std::pair<std::string, User>(nick, user) );
		++_nbUsers;
		_users[nick].tosendmsg.push_back(Message(RPL_WELCOME));
		_users[nick].tosendmsg.push_back(Message(RPL_YOURHOST));
		_users[nick].tosendmsg.push_back(Message(RPL_CREATED));
		_users[nick].tosendmsg.push_back(Message(RPL_MYINFO));
	}
	else
	{
		std::cout << "goto err\n";
		--_nbSock;
		send(_pollTab[_nbSock].fd, ERR_NICKNAMEINUSE,
			strlen(ERR_NICKNAMEINUSE), 0);
		close(_pollTab[index].fd);
		return (1);
	}
	return (0);
}

int		Server::start(void)
{
	while (loop && !(_ret))
	{
		_pollfunction();
    	//_handleMessage();
		//std::cout << "Users registered = " << _nbUsers << std::endl;
		//sleep(2);
	}
	return (_ret);
}
