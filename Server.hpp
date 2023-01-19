#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <netinet/in.h>
#include <pthread.h>
#include <vector>
#include "Thread.hpp"

class	Server
{
	private:
			int				_socketServer;
			socklen_t				_clientSize;
			struct sockaddr_in		_addrServer;
			struct sockaddr_in		_addrClient;
			std::vector<Thread>		_clients;


	public:
			std::vector<std::string>		messages;
			pthread_mutex_t			protect_messages;
			pthread_mutex_t			protect_socket;
			int				_socketClient;
					Server(void);
					
					~Server(void);
			int		start(void);
};

#endif
