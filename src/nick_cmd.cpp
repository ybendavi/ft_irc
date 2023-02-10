#include "Server.hpp"

//dans le cas ou un user deja present desire chnger son nickname, il faut imperativement preciser son
//ancien nom en argument, cela permet de faire la disctinction entre les anciens et les nouveaux
//il se passe sous la forme d'un pointeur afin de pouvoir check NULL, cela permet d avoir un pseudo vide 

//de la meme maniere, dans le cas ou l'user n est pas enregistre, il convient de passer pollfd et sockaddr
//pour que la class user resultante ne soit pas initiee dans le vide 

//comme il s'agit d'une commande aui peut rejeter un utilisateur, sa valeur de retour sera egalement 
//l'erreur generee ; il conviendra donc de passer son retour a toSend.push_back() dans les utilisateurs
//existants

User		*Server::nick_holder(User * user) 
{
	const std::vector<std::string>	&params = user->receivedmsg.front().getParams();
	
	if (params.size() < 1)
	{
		user->tosendmsg.push_back(Message (ERR_NONICKNAMEGIVEN));
		return (user);
	}
	std::string	ret;

	ret = nick_cmd(params[0], user->getNickname(), NULL, NULL);
	if (ret.empty())
		return (&(_users.find(params[0])->second));
	user->tosendmsg.push_back(Message(ret));
	return (user);
}

std::string	Server::nick_cmd(std::string nick, std::string oldnick,
		struct pollfd * fd, struct sockaddr * addr)
{
	std::map<std::string, User>::iterator	it;

	if (nick.empty())
		return (ERR_NONICKNAMEGIVEN);
	if (nick[0] == '#' || nick[0] == '&')
		return (ERR_ERRONEUSNICKNAME);
	it = _users.find(nick);
	if (it != _users.end())
		return (ERR_NICKNAMEINUSE);
	if ( oldnick.empty() )
	{
		User	newUser(fd, addr);
		if (fd == NULL || addr == NULL)
			return ("");
		it = (_users.insert(std::pair<std::string, User>(nick, newUser))).first;
		it->second.setNickname(nick);
	}
	else
	{
		_users[nick];
		std::swap(_users[nick], _users[oldnick]);
		_users[nick].setNickname(nick);
		_users.erase(oldnick);
	}
	return ("");
}
