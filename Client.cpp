#include "Client.hpp"
#include <unistd.h>
		Client::Client(void){}

		Client::Client(int socket_server)
		{
			_size = sizeof(_addr);
			_socket = accept(socket_server, (struct sockaddr *)&_addr, &_size);
			if (_socket == -1)
				perror("accept");
		}

		Client::~Client(void)
		{
			close(_socket);
		}

int		Client::getSocket(void) const
		{
			return (_socket);
		}

int		Client::sende(const void *buffer, int size)
		{
			int	ret;
			
			ret = send(_socket, buffer, size, 0);
			if (ret == -1)
				perror("send");
			return (ret);
		}

int		Client::recve(void *buffer, int size)
		{
			int	ret;

			ret = recv(_socket, buffer, size, 0);
			if (ret == -1)
				perror("send");
			return (ret);
		}
