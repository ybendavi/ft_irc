#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

class	Client
{
	private:	int			_socket;
			socklen_t		_size;
			struct sockaddr_in	_addr;
	public:
							Client(void);
							Client(int socket_server);
							~Client(void);
				int			getSocket(void) const;
};

#endif

