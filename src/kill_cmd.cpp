#include "Server.hpp"

void	Server::kill_cmd(User * user)
{
	if (!(user->getMode() & 4))
	{
		user->tosendmsg.push_back(Message(ERR_NOPRIVILEGES));
		return ;
	}

	std::vector<std::string>	params;
	
	params = user->receivedmsg.front().getParams();

	std::map<std::string, User>::iterator	it;

	it = _users.find(params[0]);
	if (it == _users.end())
	{
		user->tosendmsg.push_back(Message(ERR_NOSUCHNICK + params[0]));
		return ;
	}
	_disconnectClient((*it->second.getSocket()));
}
