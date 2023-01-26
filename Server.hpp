#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <netinet/in.h>
#include <pthread.h>
#include <vector>
#include "User.hpp"

#define MAX_USER 10;

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

			void		handleClient(int socket);



			/*handle Users*/

	private :

		std::vector<User>				_users; //might become map 
		static int						_nbUsers = 0;

	public:

		bool	find_Nickname(std::string);
		bool	find_Username(std::string);

};

#endif
