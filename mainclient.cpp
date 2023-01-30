#include <thread>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <iostream>

int	main( int ac, char **av )
{
	int sock;
	struct sockaddr_in	client;
	char	buffer[512];
	std::string	s;

	if (ac != 2)
		return (0);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(sock, F_SETFL, O_NONBLOCK);
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = htonl(INADDR_ANY);
	client.sin_port = htons(atoi(av[1]));
	connect(sock, (struct sockaddr *)&client, sizeof(client));
	while (recv(sock, buffer, 512, MSG_DONTWAIT) <= 0){}
	std::cout << buffer << std::endl;
//		send(sock, s.c_str(), s.size() + 1, 0);
	while (1)
	{
		s.clear();
		while (s.size() == 0)
//		{
			if (recv(sock, buffer, 512, MSG_DONTWAIT) > 0)
			{
				std::cout << buffer << std::endl;
				bzero(buffer, 512);
			}
			if (std::getline(std::cin, s).eof())
				std::cout << "EOF reached" << std::endl;
		}
		if (recv(sock, buffer, 512, MSG_DONTWAIT) > 0)
			{
				std::cout << buffer << std::endl;
				bzero(buffer, 512);
			}
		send(sock, s.c_str(), s.size() + 1, 0);
	}
}
