#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <netinet/in.h>
#include <pthread.h>
#include <vector>

class	Server
{
	private:
			int				_socketServer;
			socklen_t				_clientSize;
			struct sockaddr_in		_addrServer;
//			int				_socketClient;
			struct sockaddr			_addrClient;


	public:
			std::vector<std::string>		messages;
			std::vector<int>			socket_clients;
					Server(void);
					
					~Server(void);
			int		start(void);
			int		handleClient(void);
};

#endif
