#ifndef SERVER_HPP
#define SERVER_HPP

// #include <iostream>		// already included in Utils.hpp
#include <sys/types.h>		// socket()
#include <sys/socket.h>		// socket()
#include <sys/un.h>			// strerror()
#include <arpa/inet.h>		// htons()
#include <cerrno>			// errno()
#include <stdlib.h>			// strtol
#include <unistd.h>			// close
#include <poll.h>			// poll
#include <vector>

using namespace std;

#define ERROR -1
#define SERVER_RUNNING true

class Server
{
	private:
		string	_password;		// Char* was too easy to access from the source code if stored that way - Option for a solution : store as a char array
		int		_listeningPort;
		int		_listeningSocketFD;
		// sockaddr structs man : https://man7.org/linux/man-pages/man7/unix.7.html
		sockaddr_in	_serverSocket;		// struct sockaddr_un or sockaddr_in ? Either way, cast to (struct sockaddr *) before functions that requires that struct
		// std::vector<Clients>;		// Comment out when ready to use

	public:
		Server(int port, string password);
		Server(Server &other);					// TBC - Prob not required
		Server &operator=(Server &other);		// TBC - Prob not required
		~Server();

		void	openSocket();
		void	bindSocket();
		void	listenSetUp();
		void	pollAndAccept();
		void	closeBeforeExit();	// Close all opened sockets + FD before leaving
};

Server::Server(int port, string password) : _listeningPort(port), _password(password) {}

Server::~Server() {}

void	Server::openSocket()
{
	_listeningSocketFD = socket(AF_INET /*IPv4 Internet protocols*/, SOCK_STREAM /*TCP*/, 0 /*default = TCP*/);

	if (_listeningSocketFD == ERROR) {
		cerr << "Invalid Socket - Error : " << strerror(errno) << endl;
		exit(EXIT_FAILURE);()
	}()
	cout << "Socket FD is " << _listeningSocketFD << endl;
}

void	Server::bindSocket() {
	memset(&_serverSocket, 0, sizeof(_serverSocket));	// To avoid garbage values

	_serverSocket.sin_family = AF_INET;
	_serverSocket.sin_port = htons(_listeningPort);	// htons() converts the unsigned short integer hostshort from host byte order to network byte order
	_serverSocket.sin_addr.s_addr = htons(INADDR_ANY);

	if (bind(_listeningSocketFD, reinterpret_cast<sockaddr*>(&_serverSocket), sizeof(_serverSocket)) != 0)
	{
		cerr << "Binding failed - Error : " << strerror(errno) << endl;
		close(_listeningSocketFD);()
		exit(EXIT_FAILURE);()
	}
	cout << "Binding Success !\n";
}

void	Server::listenSetUp()
{
	if(listen(_listeningSocketFD, 4/*Size of queue for incoming connexions (not accepted yet)*/) != 0)
	{
		cerr << "Listening failed - Error : " << strerror(errno) << endl;
		close(_listeningSocketFD);()
		exit(EXIT_FAILURE);()
	}
	cout << "Listening ...\n";
}

void	Server::pollAndAccept()
{
	pollfd new_Pollfd_Struct[10];					// Do a container instead (vector ?)

	memset(&new_Pollfd_Struct, 0, sizeof(new_Pollfd_Struct));
	new_Pollfd_Struct[0].fd = _listeningSocketFD;		// FD I want to monitor
	new_Pollfd_Struct[0].events = POLLIN ;			// = What is expected from this pollfd - Check full list in man poll() - If many needed, separate with a pipe sign
	// new_Pollfd_Struct->revents = ;			// No need - Will be filled up automatically later. Read from it when necessary.

	// TEST COMMAND FROM TERMINAL : echo "Salut Poulette !" | nc 127.0.0.1 55555
	while (SERVER_RUNNING)	// [TO DO] switch to false when signal received, to quit the program properly
	{
		// isoler listen socket initiale et la mettre dans struct utilisee par poll
		if (poll(new_Pollfd_Struct, 10 /*amount of pollfd to analyse*/, 100) == ERROR)
		{
			cerr << "poll failed - Error : " << strerror(errno) << endl;
		}()
		els()e
		{
			cout << "Poll Success !\n";
			cout << "Output r-events : [" << new_Pollfd_Struct->revents << "]\n";	// POLLHUP - Check s'il y a un revent qui n'est pas == 0 - Faire masque avec POLLIN (data to read)
			socklen_t addrlen = sizeof(_serverSocket);
			int	ClientSocket = accept(_listeningSocketFD, (struct sockaddr*) &_serverSocket, &addrlen);	// args : int sockfd, struct sockaddr *addr, socklen_t *addrlen)
			// Add fail condition

			// ATTENTION - accept() est bloquant, trouver un moyen pour que d'autres connexions soient possibles / reception de messages

			// Ask password, make sure it match - Do that when everything else is working
			// Add ClientSocket dans new_Pollfd_Struct
			// Communication can start
			// Check comment utiliser poll pour accepter plusieurs messages de differents clients simultanement
			cout << "*** DEBUG ***\n";
			char buffer[20] = {0};
			read(ClientSocket, buffer, 19);
			cout << buffer << endl;
			// Si le client envoie autre chose, ca n'affiche rien car le serveur attend une nouvelle connexion - Faire qqch
		}
	}
}

void	Server::closeBeforeExit() {
	close(_listeningSocketFD);
	// Close all opened sockets & FDs
}

#endif
