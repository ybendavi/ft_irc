#include "Server.hpp"

std::string	get_mode(User * user)
{
	std::string	mode;

	if (user.getOp())
		mode += "+o";
	if (user.getInv())
		mode += "+i";
	if (user.getWal())
		mode += "+w";
	mode += "\r\n"; 
}

void	mode_cmd(User * user)
{
	if (user->getUsername().empty)
		user->tosendmsg.push_back(Message(ERR_NOTREGISTERED));

	std::vector<std::string>	params;
	params = user.getParams;
	if (params.size() < 1)
		user->tosendmsg.push_back(Message(ERR_NEEDMOREPARAMS));
	
	if (user->getNickname() != params[0]) 
		user->tosendmsg.push_back(Message(ERR_USERSDONTMATCH));

	if (params.size() == 1)
		user->tosendmsg.push_back(get_mode(user));
	std::vector<std::string>::iterator	it;
	it = params.begin()++;
	while (it != params.end())
	{
		check
	}
	
}
