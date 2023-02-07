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

void	Server::mode_cmd(User * user)
{
	if (user->getUsername().empty())
		user->tosendmsg.push_back(Message(ERR_NOTREGISTERED));

	std::vector<std::string>	params;
	params = user->receivedmsg.front().getParams();
	if (params.size() < 1)
		user->tosendmsg.push_back(Message(ERR_NEEDMOREPARAMS));
	
	if (user->getNickname() != params[0]) 
		user->tosendmsg.push_back(Message(ERR_USERSDONTMATCH));

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
		if (*it[0] == '+')
			
		else if (*it[0] == '-')

		else
			user->tosendmsg.push_back(Message(ERR_UMODEUNKNOWNFLAG));

	}
	
}
