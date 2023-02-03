#include "Server.hpp"

//dans le cas ou un user deja present desire chnger son nickname, il faut imperativement preciser son
//ancien nom en argument, cela permet de faire la disctinction entre les anciens et les nouveaux
//il se passe sous la forme d'un pointeur afin de pouvoir check NULL, cela permet d avoir un pseudo vide 

//de la meme maniere, dans le cas ou l'user n est pas enregistre, il convient de passer pollfd et sockaddr
//pour que la class user resultante ne soit pas initiee dans le vide 

//comme il s'agit d'une commande aui peut rejeter un utilisateur, sa valeur de retour sera egalement 
//l'erreur generee ; il conviendra donc de passer son retour a toSend.push_back() dans les utilisateurs
//existants

std::string	Server::nick_cmd(std::string nick, std::string oldnick,
		struct pollfd * fd, struct sockaddr * addr)
{
	std::map<std::string, User>::iterator	it;

	if (nick.empty())
		return (ERR_NONICKNAMEGIVEN);
	it = _users.find(nick);
	if (it != _users.end())
		return (ERR_NICKNAMEINUSE);
	if ( oldnick.empty() )
	{
		User	newUser(fd, addr);
		if (fd == NULL || addr == NULL)
			perror("Une variable n'a pas ete set correctement ");
		_users.insert(std::pair<std::string, User>(nick, newUser));
		std::cout << "nick = " << nick << std::endl;
	}
	else
	{
		std::swap(_users[nick], _users[oldnick]);
		_users.erase(oldnick);
	}
	return ("");
}
