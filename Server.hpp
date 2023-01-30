#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <netinet/in.h>
#include <vector>
#include <utility>
#include <map>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "User.hpp"
#include "Replies.hpp"

#define MAX_USER 10
#define MAX_CONN 12

class	Server
{
	public:
			Server(int port);
			~Server(void);
	
			/*getters ; faire un get channel*/

			User	getUser(std::string) const;
			int		start(void);

	private :

			std::vector<std::string>		messages; //temporaire
			std::vector<int>				socket_clients; //might diseaper
//			int		handleClient(void);
			

			/*functions*/
			int								_initClient(struct pollfd socket,
												char *buffer);
			void							_pollfunction(void);
			void							_initSocket(void);
			void							_checkUser(int *ret);
		
			/*server infos*/
			struct pollfd					_socketServer;
			struct sockaddr_in				_addrServer;
			
			/*users mayhem*/
			socklen_t						_clientSize;
			struct sockaddr					_addrClient;
			struct pollfd				*	_pollTab;

			std::map<std::string, User>		_users;
			int								_nbUsers;
			int								_nbConn;

};

#endif
