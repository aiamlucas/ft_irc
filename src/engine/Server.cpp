#include "../../includes/Server.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

Server::Server(int port, const string& password) : _listeningPort(port), _password(password) {}

Server::~Server() {}

bool	Server::openSocket()
{
	_listeningSocketFD = socket(AF_INET /*IPv4 Internet protocols*/, SOCK_STREAM /*TCP*/, 0 /*default = TCP*/);

	if (_listeningSocketFD == SYSCALL_ERROR) {
		cerr << "Invalid Socket - Error : " << strerror(errno) << endl;
		return false;
	}
	cout << "Socket FD is " << _listeningSocketFD << endl;
	return true;
}

bool	Server::bindSocket() {
	memset(&_serverSocket, 0, sizeof(_serverSocket));	// To avoid garbage values

	_serverSocket.sin_family = AF_INET;
	_serverSocket.sin_port = htonl(_listeningPort);	// htonl() converts the unsigned short integer hostshort from host byte order to network byte order
	_serverSocket.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(_listeningSocketFD, reinterpret_cast<sockaddr*>(&_serverSocket), sizeof(_serverSocket)) != 0)
	{
		cerr << "Binding failed - Error : " << strerror(errno) << endl;
		close(_listeningSocketFD);
		return false;
	}
	cout << "Binding Success !\n";
	return true;
}

bool	Server::listenSetUp()
{
	if(listen(_listeningSocketFD, 4/*Size of queue for incoming connexions (not accepted yet)*/) != 0)
	{
		cerr << "Listening failed - Error : " << strerror(errno) << endl;
		close(_listeningSocketFD);
		return false;
	}
	cout << "Listening ...\n";
	return true;
}

void	Server::pollAndAccept()
{
	// std::vector<pollfd> AllPollFDStructs;
	// AllPollFDStructs.push_back

	pollfd new_Pollfd_Struct[10];					// Do a container instead (vector ?)

	memset(&new_Pollfd_Struct, 0, sizeof(new_Pollfd_Struct));
	new_Pollfd_Struct[0].fd = _listeningSocketFD;	// FD I want to monitor
	new_Pollfd_Struct[0].events = POLLIN ;			// = What is expected from this pollfd - Check full list in man poll() - If many needed, separate with a pipe sign
	// new_Pollfd_Struct->revents = ;				// No need - Will be filled up automatically later. Read from it when necessary.

	// TEST COMMAND FROM TERMINAL : echo "Salut Poulette !" | nc 127.0.0.1 55555
	while (SERVER_RUNNING)	// [TO DO] switch to false when signal received, to quit the program properly
	{
		// isoler listen socket initiale et la mettre dans struct utilisee par poll
		if (poll(new_Pollfd_Struct, 10 /*amount of pollfd to analyse*/, 100) == SYSCALL_ERROR)
		{
			cerr << "poll failed - Error : " << strerror(errno) << endl;
		}
		else
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
			read(ClientSocket, buffer, 19);			// TESTTING / DEBUG ONLY - Delete when everything is functional
			cout << buffer << endl;
			// Si le client envoie autre chose, ca n'affiche rien car le serveur attend une nouvelle connexion - Faire qqch
		}
	}
}

void	Server::closeBeforeExit() {
	close(_listeningSocketFD);
	// Close all opened sockets & FDs
}
