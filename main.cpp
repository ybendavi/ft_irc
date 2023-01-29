#include "Server.hpp"
#include <stdlib.h>

int	main(int ac, char **av)
{
	if (ac != 2)
		return (0);
	Server	serveur(atoi(av[1])); //pense a trouver une fonction c++

	serveur.start();
	return 0;
}
