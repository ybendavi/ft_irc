#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <netinet/in.h>

class	Server
{
	private:
			int			socketServer;
			struct sockaddr_in	addrServer;

	public:
					Server(void);
					Server(const  Server&);
					~Server(void);
};

#endif
