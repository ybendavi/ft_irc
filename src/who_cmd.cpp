#include "Server.hpp"

void	Server::who_is_user(User * user, std::string s)
{
	std::map<std::string, User>::iterator	it;

	it = _users.find(s);
	if (it == _users.end() || (it->second.getMode() & 1))
	{
		user->tosendmsg.push_back(Message(ERR_NOSUCHNICK + s + ": No such nick"));
		return ;
	}
	/*glisser un channel finder ici*/
	user->tosendmsg.push_back(Message(RPL_WHOREPLY + /*channel +*/ it->second.getUsername()
				+ " " + _domainName + " " + it->second.getNickname() + " :0 "
				+ it->second.getRealname()) );
}

void	Server::who_is_channel(User * user, std::string s)
{
    std::map<std::string, Channel>::iterator    chan;

	chan = _channels.find(s);
	if (chan == _channels.end())
    {
        user->tosendmsg.push_back(Message(std::string(ERR_NOSUCHCHANNEL) + " " + s + " :No such channel"));
        return ;
    }
	std::string	rpl;
/* afficher aue s il est invisible
	while (chan.second 
	rpl = RPL_WHOREPLY + " " + s + " " + user.getUsername()
		+ _domainName + ;
*/
}

void	Server::who_cmd(User * user)
{
	std::vector<std::string>	params;
	
	params = user->receivedmsg.front().getParams();
	if (params.empty())
	{
		user->tosendmsg.push_back(Message(RPL_ENDOFWHO));
		return ;
	}
	if (params[0][0] == '#' || params[0][0] == '&')
		who_is_channel(user, params[0]);
	else
		who_is_user(user, params[0]);
}
