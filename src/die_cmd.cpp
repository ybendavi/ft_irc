#include "Server.hpp"

extern volatile sig_atomic_t loop;

void	Server::die_cmd(User * user)
{
	if (!(user->getMode() & 4))
	{
		user->tosendmsg.push_back(Message(ERR_NOPRIVILEGES));
		return ;
	}
	loop = 0;
}
