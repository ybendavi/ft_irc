#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <netinet/in.h>
#include <thread>
#include <vector>
#include "User.hpp"

#define MAX_USER 10;

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












			/*handle Users*/

	private :

		std::vector<User>				_users;
		static int						_nbUsers = 0;

	public:

		bool	find_Nickname(std::string);
		bool	find_Username(std::string);
};

#endif
