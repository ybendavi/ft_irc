#include "Server.hpp"

std::string	gnm(std::string & buff, std::string & s)
{
	size_t	i;

	i = buff.find("\r\n");
	if ( !(i != std::string::npos) )
		return ("");
	s = buff;
	s.resize(i + 2);
	if (!buff.compare("\r\n"))
		buff.erase();
	else
		buff.erase(0, i + 2);
	return (s);
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
