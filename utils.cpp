#include "Server.hpp"

std::string	findNick(std::string buffer)
{
	int	begin;

	begin = buffer.find("NICK ");
	return ( buffer.substr(begin + 5, buffer.find("\r\n", begin) ));
}
