#include "Server.hpp"
#include "signal.hpp"
#include <stdlib.h>

volatile sig_atomic_t loop = 1;

int		printErr(int ret)
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
		case 8: std::cerr << "setsockopt for SO_REUSADDR " << std::endl; break ;
		case 9: std::cerr << "setsockopt for IPV6_V6ONLY " << std::endl; break ;
		case 10: std::cerr << "accept function " << std::endl; break ;
		case 11: std::cerr << "new socket fcntl " << std::endl; break ;
		case 12: std::cerr << "poll function " << std::endl; break ;
		case 13: std::cerr << "Server is dead." << std::endl; break ;
		case 14: std::cerr << "Password cannot be empty" << std::endl; break ;
		case 15: std::cerr << "Invalid password caracter" << std::endl; break ;
		default: std::cerr << "unknown error" << std::endl;
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
