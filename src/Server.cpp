#include "Server.hpp"

extern volatile sig_atomic_t loop;

/****	Server begin and Server end ***/

Server::Server(void) : _domainName("IRCrash"), _ret(0),
	_clientSize(sizeof(struct sockaddr) ),
	_addrInfo(), _pollTab(), _tempRpl(),
	_leftover(),  _passTab(), _users(), _channels(), 
	_nbUsers(0), _nbSock(0), _on(1), _off(0)
{
	cmd_map[std::string("USER")] = &Server::cmd_user;
	cmd_map[std::string("MODE")] = &Server::mode_cmd;
	cmd_map[std::string("OPER")] = &Server::oper_cmd;
	cmd_map[std::string("NOTICE")] = &Server::_notice;
	cmd_map[std::string("PRIVMSG")] = &Server::_privMsg;
	cmd_map[std::string("QUIT")] = &Server::_quit;
	cmd_map[std::string("KILL")] = &Server::kill_cmd;
	cmd_map[std::string("PING")] = &Server::_pong;
	cmd_map[std::string("JOIN")] = &Server::_join;
	cmd_map[std::string("PART")] = &Server::_part;
	cmd_map[std::string("TOPIC")] = &Server::_topic;
	cmd_map[std::string("LIST")] = &Server::_list;
	cmd_map[std::string("NAMES")] = &Server::_names;
	cmd_map[std::string("DIE")] = &Server::die_cmd;
	cmd_map[std::string("WHO")] = &Server::who_cmd;
	cmd_map[std::string("KICK")] = &Server::_kick;
}

Server::~Server(void)
{
	while (_nbSock > 0)
	{
		--_nbSock;
		if (_pollTab[_nbSock].fd != -1)
			close(_pollTab[_nbSock].fd);
	}
}

int	Server::init(int port, std::string pass)
{
	if (!(port >= 1 && port <= 65535))
		return (-7);
	{
		if (pass.empty())
			return (-14);
		if (pass.find_first_not_of("QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm0123456789")
				!= std::string::npos)
			return (-15);
	}

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
	if (listen(_pollTab[0].fd, 0))
		return (-4);
	_pass = pass;
	_port = port;

	return (0);
}

void	Server::_reset(void)
{
	while (_nbSock > 1)
	{
		--_nbSock;
		if (_pollTab[_nbSock].fd != -1)
			close(_pollTab[_nbSock].fd);
	}
	_users.clear();
	_channels.clear();
	_nbUsers = 0;
}

int		Server::start(void)
{
	while (loop && !(_ret))
	{
		try
		{	_pollfunction(); }
		catch (std::bad_alloc &e)
		{
			std::cout << e.what() 
				<< " :: Too many data on Server!\n Rebooting...." << std::endl;
			_reset();
			
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
			_reset();
		}
	}
	return (_ret);
}

/*** User registration ***/

void	Server::_handlePass(int index, Message msg)
{
	{
		if (msg.getCommand().compare("PASS"))
		{
			_pollTab[index].events = POLLIN | POLLOUT;
			_tempRpl[index] = ERR_NOTREGISTERED;
			return ;
		}
		if (msg.getParams().empty())
		{
			_pollTab[index].events = POLLIN | POLLOUT;
			_tempRpl[index] = ERR_NEEDMOREPARAMS;
			return ;
		}
		if (msg.getParams()[0].compare(_pass))
		{
			_pollTab[index].events = POLLIN | POLLOUT;
			_tempRpl[index] = ERR_PASSWDMISMATCH;
			return ;
		}
		_passTab[index] = true;
	}
}

void	Server::_unrgUser(int index, std::string buffer)
{
	Message	msg(buffer);

	if (!msg.getCommand().compare("CAP"))
		return ;
	if (_passTab[index] == false)
		return (_handlePass(index, msg));
	
	_pollTab[index].events = POLLIN | POLLOUT;
	if (msg.getCommand().compare("NICK") && msg.getCommand().compare("QUIT"))
	{
		_tempRpl[index] = ERR_NOTREGISTERED;
		return ;
	}
	if (!msg.getCommand().compare("NICK") && msg.getParams().size() > 0)
		_tempRpl[index] = nick_cmd(msg.getParams()[0], "", &(_pollTab[index]), &(_addrInfo[index]) );
	else if (!msg.getCommand().compare("QUIT"))
		_disconnectClient(_pollTab[index]);
	else
		_tempRpl[index] = ERR_NONICKNAMEGIVEN;
}

void		Server::_initSocket(void)
{
	if (_nbSock == MAX_CONN - 1)
	{
		_pollTab[0].events = 0;
		return ;
	}
	_pollTab[_nbSock].fd  = accept(_pollTab[0].fd, (struct sockaddr* )&_addrInfo[_nbSock],
			&_clientSize);	
	if (_pollTab[_nbSock].fd  == -1)
		return ;
	if (fcntl(_pollTab[_nbSock].fd , F_SETFL, O_NONBLOCK) == -1)
	{
		close(_pollTab[_nbSock].fd);
		_ret = -11;
		return ;
	}
	_leftover[_nbSock].erase();
	_tempRpl[_nbSock].erase();
	_passTab[_nbSock] = false;
	_pollTab[_nbSock].events = POLLIN;
	_pollTab[_nbSock].revents = 0;
	++_nbSock;
}

/*** Handle commands ***/

void	Server::_execute(User *user)
{
	std::map<std::string, func_ptr>::iterator	it;

	if (user->receivedmsg.empty() == true)
		return ;
	if (user->getUsername().empty() && user->receivedmsg.front().getCommand().compare("USER"))
	{
		user->receivedmsg.pop_front();
		user->tosendmsg.push_back(Message(ERR_NOTREGISTERED));
		user->setEvent(POLLIN | POLLOUT);
		return ;
	}
	it = cmd_map.find(user->receivedmsg.front().getCommand());
	if (it != cmd_map.end())
	{
		(this->*(it->second))(user);
		if ( !(it->first.compare("QUIT")) || _ret == 1)
		{
			_ret = 0;
			return ;
		}
	}
	else if (user->receivedmsg.front().getCommand().compare("NICK") == 0)
		user = nick_holder(user);
	else
		user->tosendmsg.push_back(Message(ERR_UNKNOWNCOMMAND
					+ user->receivedmsg.front().getCommand() + " :Unknown command" ));	
	user->receivedmsg.pop_front();
	if (!user->tosendmsg.empty())
		user->setEvent(POLLIN | POLLOUT);
}

void	Server::_ft_Pollin(unsigned int i, iterator it)
{
	int			status;
	char		buffer[513];

	bzero(buffer, 513);
	status = recv(_pollTab[i].fd, buffer, 512, MSG_DONTWAIT);
	if (status <= 0)
	{
		if (status == -1)
			_ret = -5;
		_disconnectClient(_pollTab[i]);
		return;
	}
	_leftover[i] += buffer;
	bzero(buffer, strlen(buffer));
	std::string s;
	while ( !gnm(_leftover[i], s).empty() )
	{
//		std::cout << "i = "<< i << " , s = " << s << std::endl;
		if ( it != _users.end() )
		{
			it->second.receivedmsg.push_back(Message(s));
			_execute(&(it->second));
		}
		else
			_unrgUser(i, s) ;
		it = _findUserByFd(_pollTab[i].fd);
		s.erase();
	}
}

void	Server::_ft_Pollout(unsigned int i, iterator it)
{
	std::string str;
	
	if ( it != _users.end() )
	{
		if (!it->second.tosendmsg.empty())
		{
			str = it->second.tosendmsg.front().getToSend();
			readySendy(str, _domainName, it->second.getNickname());
			it->second.tosendmsg.pop_front();
		}
		else
			it->second.setEvent(POLLIN);
	}
	else
	{
		str = _tempRpl[i];
		if (str.empty())
			return ;
		readySendy(str, _domainName, "");
		_tempRpl[i].clear();
		_pollTab[i].events = POLLIN ;
	}
//	std::cout << "i = "<< i << " , to send = " << str << std::endl;
	if ( !str.empty() &&  send(_pollTab[i].fd, str.c_str(),
				strlen(str.c_str()), MSG_DONTWAIT) == -1)
		_ret = -6;
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

	ret = poll(_pollTab, _nbSock, -1);
	if (ret == -1)
		_ret = -12;
	if (_ret)
		return ;
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

/*** Handle Users ***/

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

const std::string	Server::_findUserByUsername(const std::string& username)
{
	iterator	user = _users.begin();
	std::string	nickname;

	for (; user != _users.end(); user++)
	{
		if (user->second.getUsername() == username)
		{
			nickname = user->first;
			break ;
		}
	}
	return (nickname);
}

void	Server::_removeUserFromChannels(const std::string& nickname)
{
	std::map<std::string, Channel>::iterator	it = _channels.begin();
	std::string									toSend;
	iterator									itUser;

	itUser = _users.find(nickname);

	while (it != _channels.end())
	{
		if (it->second.isUserOnChannel(nickname))
		{
			it->second.removeUserFromChannel(nickname);
			toSend = ":" + nickname + "!~" + itUser->second.getUsername() + "@hostname QUIT :\"leaving\"";
			sendMessageToAllChan(&(it->second), _users.begin(), _users.end(),toSend);
		}
		if (it->second.size() == 0)
		{
			std::cout << "last user leaving channel, destroying " << it->first << std::endl;
			_channels.erase(it++);
			continue ;
		}
		it++;
	}
}

void	Server::_disconnectClient(pollfd& client)
{
	iterator				cli;
	unsigned int			i;


	cli = _findUserByFd(client.fd);
	if (cli != _users.end())
	{
		_removeUserFromChannels(cli->first);
		_users.erase(cli);
		--_nbUsers;
	}
	for (i = 0; i < _nbSock; ++i)
	{
		if (_pollTab[i].fd == client.fd)
			break ;
	}
	close(client.fd);
	--_nbSock;
	_pollTab[i] = _pollTab[_nbSock];
	_leftover[i] = _leftover[_nbSock];
	_tempRpl[i] = _tempRpl[_nbSock];
	_addrInfo[i] = _addrInfo[_nbSock];
	_passTab[i] = _passTab[_nbSock];
	
	_pollTab[_nbSock].events = 0;
	_pollTab[_nbSock].revents = 0;

	
	cli = _findUserByFd(_pollTab[i].fd);
	cli->second.setSocket(&(_pollTab[i]));


	if (_pollTab[0].events == 0)
		_pollTab[0].events = POLLIN;
}
