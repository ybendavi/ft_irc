#include "Server.hpp"

//rfc2812 says that there's no case where a buffer bigger than 512 is taken, should we leave this
//that way?

std::string	gnm(std::string & buff)
{
	size_t	i;
	std::string	ret;

	if (buff.size() >= 512)
		buff.resize(510);
	i = buff.find("\r\n");
	if ( !(i != std::string::npos) )
	{
		buff.push_back('\r');
		buff.push_back('\n');
		i = buff.find("\r\n");
	}
	ret = buff;
	ret.resize(i + 2);
	if (!buff.compare("\r\n"))
		buff.erase();
	else
		buff.erase(0, i + 2);
	return ( ret );
}

void	readySendy(std::string &str, std::string domain, std::string nick)
{
	if (str[0] != ':' && str.compare(0, 3, "PONG", 0, 3))
	{
		std::string temp = " ";

		temp += nick;
		str.insert(3, temp);
		temp.erase();
		temp = ":";
		temp += domain + " ";
		str.insert(0, temp);
	}	
	str += "\r\n";
	//std::cout << "before send = " << str;
}
