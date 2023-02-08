#include "Server.hpp"

void	Server::oper_cmd(User * user)
{
	std::vector<std::string>	params;
	
	params = user->receivedmsg.front().getParams();
	if (params.size() < 2)
	{
		user->tosendmsg.push_back(Message(ERR_NEEDMOREPARAMS));
		return ;
	}
	if (params[1] != _pass)
	{
		user->tosendmsg.push_back(Message(ERR_PASSWDMISMATCH));
		return ;
	}
	if (params[0] != user->getNickname())
	{
		user->tosendmsg.push_back(Message(ERR_USERSDONTMATCH));
		return ;
	}
	char	mode;

	mode = user->getMode();
	user->setMode(mode |= 4);
	user->tosendmsg.push_back(Message (RPL_YOUREOPER));
	get_mode(user);
}
