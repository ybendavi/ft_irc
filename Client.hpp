#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>

class	Client
{
	private:	int			_socket;
			socklen_t		_size;
			struct sockaddr_in	_addr;
	public:
							Client(void);
							Client(int socket_server, int socket_client);
							~Client(void);
				int			getSocket(void) const;
				int			sende(const void *buffer, int size);
				int			recve(void *buffer, int size);
};

#endif

