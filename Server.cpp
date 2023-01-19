#include "Server.hpp"
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "Thread.hpp"
#include "Client.hpp"

		Server::Server(void)
		{
			_socketServer = socket(AF_INET, SOCK_STREAM, 0);
			_addrServer.sin_family = AF_INET;
			_addrServer.sin_addr.s_addr = htonl(INADDR_ANY);
			_addrServer.sin_port = htons(6667);
			bind(_socketServer, (const struct sockaddr *)&_addrServer, sizeof(_addrServer));
			listen(_socketServer, 10);
			

		}

		Server::~Server(void)
		{
			close(_socketServer);
		}

void		*handleClient(void *server)
		{
			char	buffer[50];
			int	sock;
			std::string	last;
			Server	*serv;

			serv = (Server*)server;
			sock = (*(serv->_socketClient.end() - 1)).getSocket();
			std::cout << sock << std::endl;
			send(sock, "Bienvenu\n", 9, 0);
			pthread_mutex_lock(&(serv->protect_messages));
			last = "";
			bzero(buffer, 50);
			if (!serv->messages.empty())
				last = *(serv->messages.end() - 1);
			pthread_mutex_unlock(&(serv->protect_messages));
			while (1)
			{
				pthread_mutex_lock(&(serv->protect_messages));
				if (!serv->messages.empty())
				{
					if (last.empty() || last.compare(*(serv->messages.end() - 1)) != 0)
					{
						last = *(serv->messages.end() - 1);
						send(sock, last.c_str(), 50, 0);
						std::cout << last << std::endl;
					}
				}
				pthread_mutex_unlock(&(serv->protect_messages));
				recv(sock, buffer, 50, 0);
				if (buffer && buffer[0])
				{
				std::cout << "1";				
					pthread_mutex_lock(&(serv->protect_messages));
					serv->messages.push_back(buffer);
					pthread_mutex_unlock(&(serv->protect_messages));
					bzero(buffer, 50);
				}
			}
		}

int		Server::start(void)
		{
			pthread_mutex_init(&protect_messages, NULL);
			while (1)
			{
			_socketClient.push_back(Client(_socketServer));
				if ((*(_socketClient.end() - 1)).getSocket() >= 0)
				{
					std::cout << (*(_socketClient.end() - 1)).getSocket() << std::endl;
					_clients.push_back(Thread(handleClient, (void *)this));
				}
			}
		}


