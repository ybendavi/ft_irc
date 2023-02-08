#include "Server.hpp"

//rfc2812 says that there's no case where a buffer bigger than 512 is taken, should we leave this
//that way?

std::string	gnm(std::string & buff)
{
	size_t	i;
	std::string	ret;

	ret = buff;
	if (buff.size() >= 512)
		buff.resize(510);
	i = buff.find("\r\n");
	if ( !(i != std::string::npos) )
	{
		buff.push_back('\r');
		buff.push_back('\n');
		i = buff.find("\r\n");
		std::cout << "it goes in tho \n";
	}
	ret.resize(i + 2);
	if (!buff.compare("\r\n"))
		buff.erase();
	else
		buff.erase(0, i + 2);
	std::cout << "buff before out = " << buff << std::endl;
	return ( ret );
}
