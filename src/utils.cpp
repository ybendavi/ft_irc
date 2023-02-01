#include "Server.hpp"

std::string	findNick(std::string buffer)
{
	int	begin;
	int	end;

	begin = buffer.find("NICK ");
	end = buffer.find("\r\n");
	return ( buffer.substr(begin + 5, end - begin ));
}
