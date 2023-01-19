#include "Client.hpp"
#include <unistd.h>
		Client::Client(void){}

		Client::Client(int socket_server)
		{
			_size = sizeof(_addr);
			_socket = accept(socket_server, (struct sockaddr *)&_addr, &_size);
		}

		Client::~Client(void)
		{
			close(_socket);
		}

int		Client::getSocket(void) const
		{
			return (_socket);
		}
