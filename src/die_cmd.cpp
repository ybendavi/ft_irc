#include "Server.hpp"

void	Server::die_cmd(User * user)
{
	if (!(user->getMode() & 4))
	{
		user->tosendmsg.push_back(Message(ERR_NOPRIVILEGES));
		return ;
	}
	_ret = 13;
}
