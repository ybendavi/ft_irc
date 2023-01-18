#include "User.hpp"
#include <iostream>

int	main()
{
	User coucou;

	coucou.setRealname("slt");
	std::cout << coucou.getRealname() << std::endl;
	std::cout << RPL_WELCOME << std::endl;
}
