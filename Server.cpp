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

		Server::Server(int port)
		{
			_socketServer = socket(AF_INET, SOCK_STREAM, 0);
			fcntl(_socketServer, F_SETFL, O_NONBLOCK);
			_addrServer.sin_family = AF_INET;
			_addrServer.sin_addr.s_addr = htonl(INADDR_ANY);
			_addrServer.sin_port = htons(port);
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
					std::cout << "i = " << i << std::endl;
					messages.push_back(std::string(buffer));
					std::cout << buffer << std::endl;
					bzero(buffer, 512);
					send(socket_clients[i], RPL_WELCOME, sizeof(RPL_WELCOME), 0);
			}	
		/*		if (recv(socket_clients[i], buffer, 512, MSG_DONTWAIT) > 0)
				{
					messages.push_back(std::string(buffer));
					std::cout << buffer << std::endl;
					bzero(buffer, 512);
				}

				send(socket_clients[i], RPL_YOURHOST, sizeof(RPL_YOURHOST), 0);
				
				if (recv(socket_clients[i], buffer, 512, MSG_DONTWAIT) > 0)
				{
					messages.push_back(std::string(buffer));
					std::cout << buffer << std::endl;
					bzero(buffer, 512);
				}

				send(socket_clients[i], RPL_CREATED, sizeof(RPL_CREATED), 0);
				
				if (recv(socket_clients[i], buffer, 512, MSG_DONTWAIT) > 0)
				{
					messages.push_back(std::string(buffer));
					std::cout << buffer << std::endl;
					bzero(buffer, 512);
				}

				send(socket_clients[i], RPL_MYINFO, sizeof(RPL_MYINFO), 0);
					
				if (recv(socket_clients[i], buffer, 512, MSG_DONTWAIT) > 0)
				{
					messages.push_back(std::string(buffer));
					std::cout << buffer << std::endl;
					bzero(buffer, 512);
				}*/
				i++;
			}
			return (0);
		}

void	Server::_pollfunction(std::map<std::string, User>::iterator user) 
{
	struct	pollfd	tab;

	std::cout << "i'm in" << std::endl;
	tab.fd = user->second.getSocket();
	tab.events = POLLOUT;

	poll(&tab, 1, 10000000);
	std::cout << tab.events << " ; " << tab.revents << std::endl;

}

void	Server::initClient(int socket)
{
	User						user(socket);
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
	//	while (recv(socket, buffer, 512, MSG_DONTWAIT) < 1)
		{
			std::cout << "0\n";
			_pollfunction(tmp.first);
			if (send(socket, RPL_WELCOME, sizeof(RPL_WELCOME), MSG_DONTWAIT) == -1)
				std::cout << "err1\n";
			std::cout << buffer << "1" << std::endl;
			bzero(buffer, 512);
			_pollfunction(tmp.first);
			if (send(socket, RPL_YOURHOST, sizeof(RPL_YOURHOST), MSG_DONTWAIT) == -1)
				std::cout << "err2\n";
			std::cout << buffer << "2" << std::endl;
			bzero(buffer, 512);
			_pollfunction(tmp.first);
			if (send(socket, RPL_CREATED, sizeof(RPL_CREATED), MSG_DONTWAIT) == -1)
				std::cout << "err3\n";
			std::cout << buffer << "3" << std::endl;
			bzero(buffer, 512);
			_pollfunction(tmp.first);
			if (send(socket, RPL_MYINFO, sizeof(RPL_MYINFO), MSG_DONTWAIT) == -1)
				std::cout << "err4\n";
			std::cout << buffer <<  "4" << std::endl;
			std::cout << "do i leave it?";
		}
		socket_clients.push_back(socket);
		std::cout << "Client with fd number " << *(socket_clients.end() - 1) << " was created." << std::endl;
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
				handleClient();
						
			}
		}


