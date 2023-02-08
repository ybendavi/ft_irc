#include "Server.hpp"

//rfc2812 says that there's no case where a buffer bigger than 512 is taken, should we leave this
//that way?

std::string	gnm(std::string & buff)
{
	int	i;
	std::string	ret;

	ret = buff;
	if (buff.size() >= 512)
	{
		buff[511] = '\n';
		buff[510] = '\r';
		buff.resize(512);
	}
	i = buff.find("\r\n");
	if (i == -1)
		return ("cKasse");
	ret.resize(i + 2);
	buff.erase(0, i + 2);
	return ( ret );
}
