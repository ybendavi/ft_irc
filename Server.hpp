#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <netinet/in.h>
#include <thread>
#include <vector>

class	Server
{
	private:
			int				_socketServer;
			int				_socketClient;
			socklen_t				_clientSize;
			struct sockaddr_in		_addrServer;
			struct sockaddr_in		_addrClient;
			std::vector<std::thread>	_clients;


	public:
					Server(void);
					
					~Server(void);
			int		start(void);
			void		handleClient(int socket);
};

#endif
