#include "Server.hpp"

extern volatile sig_atomic_t loop;

Server::Server(void) : _ret(0), _clientSize(sizeof(struct sockaddr) ),
	_addrInfo(), _pollTab(), _tempRpl(), _users(), _channels(),
	_nbUsers(0), _nbSock(0), _on(1), _off(0)
{ }

Server::~Server(void)
{
	while (_nbSock > 0)
	{
		--_nbSock;
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

	if (listen(_pollTab[0].fd, 2))
		return (-4);

	return (0);
}

void	Server::_unrgUser(int index, std::string buffer)
{
	Message	msg(buffer);

	if (!msg.getCommand().compare("CAP"))
		return ;
	_pollTab[index].events = POLLIN | POLLOUT;
	if (msg.getCommand().compare("NICK") && msg.getCommand().compare("QUIT"))
	{
		_tempRpl[index] = ERR_NOTREGISTERED;
		return ;
	}
	if (!msg.getCommand().compare("NICK") && msg.getParams().size() > 0)
		_tempRpl[index] = nick_cmd(msg.getParams()[0], "", &(_pollTab[index]), &(_addrInfo[index]) );
	else
		_tempRpl[index] = ERR_NONICKNAMEGIVEN;
		
	//coder quit ;)
}

void	Server::_ft_Pollin(unsigned int i, iterator it)
{
	char		buffer[512];

	bzero(buffer, 512);
	if (recv(_pollTab[i].fd, buffer, 512, MSG_DONTWAIT) == -1)
	{
		_ret = -5;
		return;
	}
//	std::cout << "buffer = " << buffer << std::endl;
	std::string buff(buffer);
	bzero(buffer, strlen(buffer));
	while (!buff.empty()) // si msg coupes go here
	{
		if ( it != _users.end() )
		{
			it->second.receivedmsg.push_back(Message( gnm(buff) ));
			_execute(&(it->second));
		}
		else
			_unrgUser(i, gnm(buff) );
		it = _findUserByFd(_pollTab[i].fd);
	}
	buff.erase();
}

void	Server::_ft_Pollout(unsigned int i, iterator it)
{
	if ( it != _users.end() )
	{
		if (!it->second.tosendmsg.empty())
		{
			std::string str = it->second.tosendmsg.front().getToSend();

			if (send(_pollTab[i].fd, str.c_str(),
					strlen(str.c_str()), MSG_DONTWAIT) == -1)	
				_ret = -6;
			it->second.tosendmsg.pop_front();
		}
		else
			it->second.setEvent(POLLIN);
	}
	else
	{
		if (!_tempRpl[i].empty() && send(_pollTab[i].fd, _tempRpl[i].c_str(),
				strlen(_tempRpl[i].c_str()), MSG_DONTWAIT) == -1)
			_ret = -6;
		_tempRpl[i].clear();
		_pollTab[i].events = POLLIN ;
	}
}


void	Server::_checkUser(void)
{
	unsigned int							i;
	std::map<std::string, User>::iterator	it;

	i = 1;
	while (i < _nbSock)
	{
		it = _findUserByFd(_pollTab[i].fd);
		if ((_pollTab[i].revents & 1) == POLLIN)
			_ft_Pollin(i, it);
		if (_ret)
			return ;
		it = _findUserByFd(_pollTab[i].fd);
		if ((_pollTab[i].revents & 4) == POLLOUT)
			_ft_Pollout(i, it);
		if (_ret)
			return ;
		++i;
	}
}

void	Server::_pollfunction(void) 
{
	int		ret;

//	for (unsigned i = 0;  i < _nbSock; i++)
//			std::cout << "before : i = " << i << " event = " << _pollTab[i].events << "revent = " << _pollTab[i].revents << std::endl;
	ret = poll(_pollTab, _nbSock, 7000);
//	for (unsigned i = 0;  i < _nbSock; i++)
//			std::cout << "after : i = " << i << " event = " << _pollTab[i].events << "revent = " << _pollTab[i].revents << std::endl;
	if (ret == 0)
		std::cout << "Timeout\n";
	else if (ret == -1)
		_ret = -12;
	if (_ret)
		return ;
	else if ( (_pollTab[0].events & (POLLNVAL|POLLERR|POLLHUP) ))
		std::cout << "buggybugg" << std::endl;
	else
	{
		if (_nbSock > 1)
			_checkUser();
		if (_ret)
			return ;

		if (_pollTab[0].revents == POLLIN)
		{
			_initSocket();
			if (_ret)
				return ;
		}
	}
}

void		Server::_initSocket(void)
{
	_pollTab[_nbSock].fd  = accept(_pollTab[0].fd, (struct sockaddr* )&_addrInfo[_nbSock],
			&_clientSize);
	if (_pollTab[_nbSock].fd  == -1)
	{
		_ret = -10;
		return ;
	}
	if (fcntl(_pollTab[_nbSock].fd , F_SETFL, O_NONBLOCK) == -1)
	{
		close(_pollTab[_nbSock].fd);
		_ret = -11;
		return ;
	}
	_pollTab[_nbSock].events = POLLIN;
	_pollTab[_nbSock].revents = 0;
	++_nbSock;
}

int		Server::start(void)
{
	while (loop && !(_ret))
		_pollfunction();
	return (_ret);
}

void	Server::_execute(User *user)
{
	if (user->receivedmsg.empty() == true)
	{
		std::cout << "false" << std::endl;
		return ;
	}
	if (user->receivedmsg.front().getCommand().compare("PING") == 0)
	{
		std::string	pong("PONG \r\n");

		pong.insert(5, *(user->receivedmsg.front().getParams().begin()));
		user->tosendmsg.push_back(Message(pong.c_str()));
	}
	else if (user->receivedmsg.front().getCommand().compare("NOTICE") == 0)
		_notice(user);
	else if (user->receivedmsg.front().getCommand().compare("PRIVMSG") == 0)
		_notice(user);
	else if (user->receivedmsg.front().getCommand().compare("USER") == 0)
		cmd_user(user);
//	else if (user->receivedmsg.front().getCommand().compare("MODE") == 0)
//		mode_cmd(user);
/*	else
	{
		std::cout << "not handled:" << user->receivedmsg.front().getCommand() << std::endl;
	}*/
	user->receivedmsg.pop_front();
	if (!user->tosendmsg.empty())
		user->setEvent(POLLIN | POLLOUT);
}

void	Server::_notice(User *user)
{
	std::map<std::string, User>::iterator	it;
	std::vector<std::string>::iterator	ite;

	//std::cout << "ici" << std::endl;
	if (user->receivedmsg.front().getParams().empty() == true
		|| user->receivedmsg.front().getParamsopt().empty() == true)
		return ;
	//std::cout << "et ici" << std::endl;
	ite = user->receivedmsg.front().getParams().begin();
	//std::cout << "nick:" << *(user->receivedmsg.front().getParams().begin()) << std::endl;
	//std::cout << "nick of user:" << (_users.begin()++)->second.getNickname() << std::endl;
	it = getUser(*(user->receivedmsg.front().getParams().begin()));
	//if (getUser(*(user->receivedmsg.front().getParams().begin())) == _users.end())
	//	return ;
	//std::cout << "userlooking:" << _users.begin()->first << std::endl;
	//std::cout << "userlooking:" << _users.find(std::string("LS\r\n"))->second.getNickname()<< std::endl;
	if (_users.find(*(user->receivedmsg.front().getParams().begin())) == _users.end())
		return ;
	//std::cout << "encore ici" << std::endl;
	_users.find(*(user->receivedmsg.front().getParams().begin()))->second.tosendmsg.push_back(Message(user->receivedmsg.front().getMessage().c_str()));
}

void	Server::_privMsg(User *user)
{
	std::map<std::string, User>::iterator	it;
	std::vector<std::string>::iterator	ite;

	//std::cout << "ici" << std::endl;
	if (user->receivedmsg.front().getParams().empty() == true
		|| user->receivedmsg.front().getParamsopt().empty() == true)
		return ;
	//std::cout << "et ici" << std::endl;
	ite = user->receivedmsg.front().getParams().begin();
	//std::cout << "nick:" << *(user->receivedmsg.front().getParams().begin()) << std::endl;
	//std::cout << "nick of user:" << (_users.begin()++)->second.getNickname() << std::endl;
	it = getUser(*(user->receivedmsg.front().getParams().begin()));
	//if (getUser(*(user->receivedmsg.front().getParams().begin())) == _users.end())
	//	return ;
	//std::cout << "userlooking:" << _users.begin()->first << std::endl;
	//std::cout << "userlooking:" << _users.find(std::string("LS\r\n"))->second.getNickname()<< std::endl;
	if (_users.find(std::string("LS\r\n")) == _users.end())
		return ;
	//std::cout << "encore ici" << std::endl;
	//_users.find(*(user->receivedmsg.front().getParams().begin()))->second.tosendmsg.push_back(Message(user->receivedmsg.front().getMessage().c_str()));
	_users.find(std::string("LS\r\n"))->second.tosendmsg.push_back(Message(user->receivedmsg.front().getToSend().c_str()));
}

std::map<std::string, User>::iterator	Server::getUser(std::string nick)
{
	return (_users.find(nick));
}

std::map<std::string, User>::iterator	Server::_findUserByFd(int fd)
{
	iterator	user = _users.begin();

	for (; user != _users.end(); user++)
	{
		if (user->second.getSocket()->fd == fd)
			break ;
	}

	return (user);
}
