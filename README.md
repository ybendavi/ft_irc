<h2>Ft_irc</h2>
Ce projet consiste à recréer un serveur IRC (Internet Relay Chat) en C++98, un protocole de communication textuel utilisé pour des discussions instantanées. Il offre les compétences suivantes :
<br>
<br>
<ul>
<li>Maîtrise des concepts réseau : implémentation de la communication via TCP/IP, gestion des sockets, et utilisation des descripteurs de fichier non bloquants.<br></li>
<li>Développement d’un serveur multi-clients : gérer plusieurs connexions simultanément sans bloquer le serveur, avec une approche événementielle via poll() ou équivalent.<br></li>
<li>Implémentation des fonctionnalités IRC essentielles : authentification, gestion des utilisateurs et opérateurs, commandes de canal (e.g., KICK, INVITE, TOPIC, MODE).<br></li>
<li>Respect des standards : garantir l’interopérabilité avec un client IRC existant pour tester et valider le serveur.<br></li>
</ul>
Ce projet développe des compétences clés en programmation réseau et en systèmes distribués, tout en respectant des contraintes strictes de normes et de robustesse.
<br>
<br>

https://www.rfc-editor.org/rfc/rfc2812

https://www.rfc-editor.org/rfc/rfc2811 for channels

https://modern.ircdocs.horse/

https://libera.chat/guides/clients

https://hechao.li/2022/01/04/select-vs-poll-vs-epoll/

test pour nc & recup tous les retours d'un serveur "normal"

nc -CN punch.wa.us.dal.net 6667


retour la cmd stat = :<server name> 219 truc * :End of /STATS report. 
