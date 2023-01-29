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
	private:
			int				_socketServer;
			socklen_t				_clientSize;
			struct sockaddr_in		_addrServer;
//			int				_socketClient;
			struct sockaddr			_addrClient;

	public:
			std::vector<std::string>		messages;
			std::vector<int>			socket_clients;
					Server(int port);
					~Server(void);
			int		start(void);

			int		handleClient(void);

			void	initClient(int socket);


			/*handle Users*/

	private :

		std::map<std::string, User>		_users;
		int								_nbUsers;
		void							_pollfunction(std::map<std::string, User>::iterator user);

	public:

		bool	find_Nickname(std::string);
		bool	find_Username(std::string);

};

#endif
