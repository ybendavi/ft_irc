#include "Server.hpp"
#include <thread>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>

		Server::Server(void)
		{
			_socketServer = socket(AF_INET, SOCK_STREAM, 0);
			_addrServer.sin_family = AF_INET;
			_addrServer.sin_addr.s_addr = htonl(INADDR_ANY);
			_addrServer.sin_port = htons(6667);
			bind(_socketServer, (const struct sockaddr *)&_addrServer, sizeof(_addrServer));
			listen(_socketServer, 2);
			while (1)
			{
				_clientSize = sizeof(_addrClient);
				_socketClient = accept(_socketServer, (struct sockaddr *)&_addrClient, &_clientSize);
								
				if (_socketClient >= 0)
				{
					std::cout << "connected" << std::endl;
				}
			}

		}

		Server::~Server(void)
		{
			close(_socketServer);
			close(_socketClient);
		}
