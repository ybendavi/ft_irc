#include "Server.hpp"

void	get_mode(User * user)
{
	std::string	mode;

	mode = RPL_UMODEIS;
	mode += user->getNickname();
	mode += (" +");
	if (user->getOp())
		mode.push_back('o');
	if (user->getInv())
		mode.push_back('i');
	if (user->getWal())
		mode.push_back('w');
	mode += "\r\n";
	user->tosendmsg.push_back(Message(mode));
}

void	add_mode(std::string s, User * user)
{
	for (unsigned int i = 0; i < s.size(); ++i)
	{
		if (s[i] == 'i')
			user->setInv(true);
		else if (s[i] == 'w')
			user->setWal(true);
		else
			user->tosendmsg.push_back(Message(ERR_UMODEUNKNOWNFLAG));
	}
}

void	rm_mode(std::string s, User * user)
{
	for (unsigned int i = 0; i < s.size(); ++i)
	{
		if (s[i] == 'o')
			user->setOp(false);
		if (s[i] == 'i')
			user->setInv(false);
		if (s[i] == 'w')
			user->setWal(false);
		else
			user->tosendmsg.push_back(Message(ERR_UMODEUNKNOWNFLAG));
	}
}

void	Server::mode_cmd(User * user)
{
	if (user->getUsername().empty())
	{
		user->tosendmsg.push_back(Message(ERR_NOTREGISTERED));
		return ;
	}
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
			user->tosendmsg.push_back(Message(ERR_UMODEUNKNOWNFLAG));
	}
}
