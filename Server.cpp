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

		Server::Server(void)
		{
			_socketServer = socket(AF_INET, SOCK_STREAM, 0);
			fcntl(_socketServer, F_SETFL, O_NONBLOCK);
			_addrServer.sin_family = AF_INET;
			_addrServer.sin_addr.s_addr = htonl(INADDR_ANY);
			_addrServer.sin_port = htons(6668);
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
					std::cout << buffer << std::endl;
					bzero(buffer, 512);
				}

				send(socket_clients[i], RPL_WELCOME, sizeof(RPL_WELCOME), 0);
				
				if (recv(socket_clients[i], buffer, 512, MSG_DONTWAIT) > 0)
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
				}
				i++;
			}
			return (0);
		}

int		Server::start(void)
		{
			int	socket;

			_clientSize = sizeof(_addrClient);
			while (1)
			{
				socket = accept(_socketServer, (struct sockaddr* )&_addrClient, &_clientSize);
				if (socket > 0)
				{
					std::cout << "coucou" << std::endl;
					socket_clients.push_back(socket);
					std::cout << "Client with fd number " << *(socket_clients.end() - 1) << " was created." << std::endl;
				}
				handleClient();
						
			}
		}


