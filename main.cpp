#include "Server.hpp"
#include <stdlib.h>
/*
int		printErr(int ret)
{
	if (ret == -1)
		std::cerr << "Server init : socket problem : " << errno << std::endl;
	if (ret == -2)
		std::cerr << "Server init : fcntl problem : " << errno << std::endl;
	if (ret == -3)
		std::cerr << "Server init : bind problem : " << errno << std::endl;
	if (ret == -4)
		std::cerr << "Server init : listen problem : " << errno << std::endl;
	if (ret == -5)
		std::cerr << "Server problem : recv : " << errno << std::endl;
	if (ret == -6)
		std::cerr << "Server problem : send : " << errno << std::endl;
	
	return (1);
}*/

int		printErr(int ret)
{
	if (ret == -1)
		perror("Server init : socket problem " );
	if (ret == -2)
		perror("Server init : fcntl problem ");
	if (ret == -3)
		perror("Server init : bind problem ");
	if (ret == -4)
		perror("Server init : listen problem " );
	if (ret == -5)
		perror("Server problem : recv ") ;
	if (ret == -6)
		perror("Server problem : send ");
	
	return (1);
}

int		main(int ac, char **av)
{
	int	ret;

	if (ac != 2)
		return (0);
	
	Server	server;
	
	ret = server.init( atoi(av[1]) ); //gerer l'erreur pour ca aussi
	if (ret)
		return ( printErr(ret) );
	ret = server.start();
	if (ret)
		return ( printErr(ret) );
	return (0);
}
