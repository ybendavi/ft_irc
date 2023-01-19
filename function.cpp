#include "header.hpp"
#include "Server.hpp"

bool	Server::find_Nickname(std::string s)
{
	int	i;
	
	i = 0;
	while (i < _nbUsers)
	{
		if (!_users[i].getNickname().compare(s))
			return (true);
		i++;
	}
	return (false);
}

bool	Server::find_Username(std::string s)
{
	int	i;
	
	i = 0;
	while (i < _nbUsers)
	{
		if (!_users[i].getUsername().compare(s))
			return (true);
		i++;
	}
	return (false);
}

std::string	register_user(void)
{
	
}
