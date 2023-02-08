#include "Server.hpp"

void	Server::get_mode(User * user)
{
	std::string	mode;
	char		umode;

	mode = RPL_UMODEIS;
	mode += (" +");
	umode = user->getMode();
	if (umode & 4)
		mode.push_back('o');
	if (umode & 1)
		mode.push_back('i');
	if (umode & 2)
		mode.push_back('w');
	user->tosendmsg.push_back(Message(mode));
}

void	add_mode(std::string s, User * user)
{
	char	mode;

	mode = user->getMode();
	for (unsigned int i = 1; i < s.size(); ++i)
	{
		std::cout << "si = " << s[i] << std::endl;
		if (s[i] == 'i')
			mode |= 1;
		else if (s[i] == 'w')
			mode |= 2;
		else
			user->tosendmsg.push_back(Message(ERR_UMODEUNKNOWNFLAG));
	}
	user->setMode(mode);
}

void	rm_mode(std::string s, User * user)
{
	char	mode;
	
	mode = user->getMode();
	for (unsigned int i = 1; i < s.size(); ++i)
	{
		std::cout << "si = " << s[i] << std::endl;
		if (s[i] == 'o' && (mode & 4))
			mode -= 4;
		else if (s[i] == 'i' && (mode & 1))
			mode -= 1;
		else if (s[i] == 'w' && (mode & 2))
			mode -= 2;
		else
			user->tosendmsg.push_back(Message(ERR_UMODEUNKNOWNFLAG));
	}
	user->setMode(mode);
}

void	Server::mode_cmd(User * user)
{
	std::vector<std::string>	params;
	
	params = user->receivedmsg.front().getParams();
	if (params.size() < 1)
	{
		user->tosendmsg.push_back(Message(ERR_NEEDMOREPARAMS));
		return ;
	}
	
	if (user->getNickname() != params[0])
	{
		user->tosendmsg.push_back(Message(ERR_USERSDONTMATCH));
		return ;
	}

	if (params.size() == 1)
	{
		get_mode(user);
		return ;
	}
	std::vector<std::string>::iterator	it;
	it = params.begin();
	++it;
	while (it != params.end())
	{
		std::string s = *it;
		if (s[0] == '+')
			add_mode(s, user);
		else if (s[0] == '-')
			rm_mode(s, user);
		else
		{
			user->tosendmsg.push_back(Message(ERR_UMODEUNKNOWNFLAG));
			return ;
		}
		it++;
	}
	get_mode(user);
}
