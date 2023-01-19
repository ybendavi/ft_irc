#include <thread>
#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int	main()
{
	int sock;
	struct sockaddr_in	client;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = htonl(INADDR_ANY);
	client.sin_port = htons(6667);
	bind(sock, (const struct sockaddr *)&client, sizeof(client));
	connect(sock, (struct sockaddr *)&client, sizeof(client));

}
