#include "Server.hpp"
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
			_addrServer.sin_port = htons(6667);
			std::cout << bind(_socketServer, (const struct sockaddr *)&_addrServer, sizeof(_addrServer)) << std::endl;
			listen(_socketServer, 5);
			

		}

		Server::~Server(void)
		{
			close(_socketServer);
		}

int		Server::handleClient(void)
		{
			size_t	i;
			size_t	j;
			char	buffer[50];

			i = 0;
			while (i < socket_clients.size())
			{
				if (recv(socket_clients[i], buffer, 50, MSG_DONTWAIT) > 0)
				{
					//std::cout << buffer << std::endl;
					j = 0;
					messages.push_back(std::string(buffer));
					while (j < socket_clients.size())
					{
						if (j != i)
							send(socket_clients[j], (*(messages.end() - 1)).c_str(), (*(messages.end() - 1)).size(), 0);
						j++;
					}
					bzero(buffer, 50);
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
					send(socket, "Bienvenue ma poule!", 20, 0);
					socket_clients.push_back(socket);
					std::cout << "Client with fd number " << *(socket_clients.end() - 1) << " was created." << std::endl;
				}
				handleClient();
						
			}
		}


