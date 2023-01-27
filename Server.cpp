#include "Server.hpp"
#include "Replies.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include "User.hpp"

		Server::Server(void)
		{
			_socketServer = socket(AF_INET, SOCK_STREAM, 0);
			fcntl(_socketServer, F_SETFL, O_NONBLOCK);
			_addrServer.sin_family = AF_INET;
			_addrServer.sin_addr.s_addr = htonl(INADDR_ANY);
			_addrServer.sin_port = htons(6669);
			std::cout << bind(_socketServer, (const struct sockaddr *)&_addrServer, sizeof(_addrServer)) << " bind" << std::endl;
			listen(_socketServer, 5);
			

		}

		Server::~Server(void)
		{
			close(_socketServer);
		}

int		Server::handleClient(void)
		{
			size_t	i;
			char	buffer[512];
	
			i = 0;
				
			while (i < socket_clients.size())
			{		
				if (recv(socket_clients[i], buffer, 512, MSG_DONTWAIT) > 0)
				{
					messages.push_back(std::string(buffer));
				//	std::cout << buffer << std::endl;
					bzero(buffer, 512);
				}
				i++;
			}
			return (0);
		}

void	Server::initClient(int socket)
{
	User						user;
	char						buffer[512];
	std::pair<std::map<std::string, User>::iterator, bool>	tmp;
	
	if (recv(socket, buffer, 512, MSG_DONTWAIT) > 0)
		messages.push_back(std::string(buffer));
	user.parseUser(buffer);
	
	tmp = _users.insert( std::pair<std::string, User>(user.getNickname(), user) );
	std::cout << "nick = " << user.getNickname() << " user= " << user.getUsername() << " ip= " 
		<< user.getIp() << " realname= " << user.getRealname() << std::endl;
	if (tmp.second == true)
	{
		socket_clients.push_back(socket);
		std::cout << "Client with fd number " << *(socket_clients.end() - 1) << " was created." << std::endl;
		send(socket_clients[_nbUsers], RPL_WELCOME, sizeof(RPL_WELCOME), 0);
		send(socket_clients[_nbUsers], RPL_YOURHOST, sizeof(RPL_YOURHOST), 0);
		send(socket_clients[_nbUsers], RPL_CREATED, sizeof(RPL_CREATED), 0);
		send(socket_clients[_nbUsers], RPL_MYINFO, sizeof(RPL_MYINFO), 0);
		++_nbUsers;
		std::cout << "new";
	}
	else
		send(socket_clients[_nbUsers], ERR_NICKNAMEINUSE, sizeof(ERR_NICKNAMEINUSE), 0);

}

int		Server::start(void)
		{
			int	socket;

			_clientSize = sizeof(_addrClient);
			_nbUsers = 0;

			while (1)
			{
				socket = accept(_socketServer, (struct sockaddr* )&_addrClient, &_clientSize);
				if (socket > 0)
				{
					initClient(socket);
					std::cout << "Users registered = " << _nbUsers << std::endl;
				}
			//	handleClient();
						
			}
		}


