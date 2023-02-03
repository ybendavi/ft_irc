#include "Server.hpp"

std::string	gnm(std::string & buff)
{
	int	i;
	std::string	ret;

	ret = buff;
	i = buff.find("\r\n");
	ret.resize(i + 2);
	buff.erase(0, i + 2);
	return ( ret );
}
