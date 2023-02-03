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
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "User.hpp"
#include "Replies.hpp"
#include "Message.hpp"

#define MAX_USER 10
#define MAX_CONN 12


class	Server
{
	public:
			Server(void);
			~Server(void);
	
			int		init(int port);
			int		start(void);
			
			/*getters ; faire un get channel*/

			std::map<std::string, User>::iterator	getUser(std::string);

	private :

//			int		handleClient(void);

			/*functions*/
			int								_initClient(int index);
			int								_initSocket(void);
			void							_pollfunction(void);
			void							_checkUser(int *ret);
			void							_handleMessage(void);
			void							_execute(User *user);
			void							_notice(User *user);
			void							_privMsg(User *user);

		
			/*server infos*/
			struct sockaddr_in6				_addrServer;
			int								_ret;

			/*users mayhem*/
			socklen_t						_clientSize;
			struct sockaddr					_addrInfo[MAX_CONN];
			struct pollfd					_pollTab[MAX_CONN];
			unsigned short					_index; //maybe int-nutile

			std::map<std::string, User>		_users;
			int								_nbUsers;
			nfds_t							_nbSock;

			/*socket options utils*/
			int								_on;
			int								_off;

};

std::string	findNick(std::string buffer);

#endif
