#include "Server.hpp"

std::string	gnm(std::string & buff)
{
	int	i;
	std::string	ret;

	ret = buff;
	std::cout << "buff = " << buff << std::endl;
	i = buff.find("\r\n");
	if (i == -1)
		return ("cKasse");
	std::cout << " i = " << i;
	ret.resize(i + 2);
	std::cout << "; ret  = " << ret;
	buff.erase(0, i + 2);
	std::cout << "   buff en sortie = " << buff << std::endl;
	return ( ret );
}
