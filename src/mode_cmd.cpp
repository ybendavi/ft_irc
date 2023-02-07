#include "Server.hpp"

void	get_mode(User * user)
{
	std::string	mode;

	mode = RPL_UMODEIS;
	if (user->getOp())
		mode += "+o";
	if (user->getInv())
		mode += "+i";
	if (user->getWal())
		mode += "+w";
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
/*	while (it != params.end())
	{
		if (
	}*/
	
}
