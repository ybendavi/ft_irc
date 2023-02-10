#include "Server.hpp"
#include "signal.hpp"
#include <stdlib.h>

volatile sig_atomic_t loop = 1;

/*int		printErr(int ret)
{
	ret = std::abs(ret);

	switch (ret)
	{
		case 1: std::cerr << "Server init : socket problem " << std::endl; break ;
		case 2: std::cerr << "Server init : fcntl problem " << std::endl; break ;
		case 3: std::cerr << "Server init : bind problem " << std::endl; break ;
		case 4: std::cerr << "Server init : listen problem " << std::endl; break ;
		case 5: std::cerr << "Server problem : recv " << std::endl; break ;
		case 6: std::cerr << "Server problem : send " << std::endl; break ;
		case 7: std::cerr << "invalid port number/argument" << std::endl; break ;
		default: std::cerr << "unknown error" << std::endl;
	}	
	return (ret);
}*/

int		printErr(int ret)
{
	ret = std::abs(ret);

	switch (ret)
	{
		case 1: perror("Server init : socket problem "); break ;
		case 2: perror("Server init : fcntl problem "); break ;
		case 3: perror("Server init : bind problem "); break ;
		case 4: perror("Server init : listen problem "); break ;
		case 5: perror("Server problem : recv "); break ;
		case 6: perror("Server problem : send "); break ;
		case 7: perror("invalid port number/argument "); break ;
		case 8: perror("error: setsockopt for SO_REUSADDR "); break ;
		case 9: perror("error: setsockopt for IPV6_V6ONLY "); break ;
		case 10: perror("error: accept function "); break ;
		case 11: perror("error: new socket fcntl "); break ;
		case 12: perror("error: poll function "); break ;
		case 13: std::cout << "Server is dead.\n"; break ;
		case 14: std::cout << "Password cannot be empty\n"; break ;
		default: perror("unknown error");
	}	
	return (ret);
}

int		main(int ac, char **av)
{
	Server	server;
	int		ret;

	signal(SIGINT, sighandler);
	signal(SIGPIPE, sighandler);

	if (ac != 3)
	{
		std::cerr << "Error" << std::endl << "usage: ./ircserv <port> <password>" << std::endl;
		return (EXIT_FAILURE);
	}

	ret = server.init( atoi(av[1]), av[2] );
	if (ret)
		return ( printErr(ret) );
	ret = server.start();
	if (ret)
		return ( printErr(ret) );

	return (EXIT_SUCCESS);
}
