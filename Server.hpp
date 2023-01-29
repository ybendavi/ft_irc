#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <netinet/in.h>
#include <pthread.h>
#include <vector>
#include <utility>
#include <map>
#include "User.hpp"

#define MAX_USER 10;

class	Server
{
	public:
			Server(int port);
			~Server(void);
	
			/*getters ; faire un get channel*/

			User	getUser(std::string) const;
			int		start(void);

	private :

//			int		handleClient(void);
			void	initClient(int socket);

			
			std::vector<std::string>		messages; //temporaire
			std::vector<int>				socket_clients; 
			int								_socketServer;
			socklen_t						_clientSize;
			struct sockaddr_in				_addrServer;
			struct sockaddr					_addrClient;

			std::map<std::string, User>		_users;
			int								_nbUsers;
			void							_pollfunction(std::map<std::string, User>::iterator user);

};

#endif
