#include "../../includes/Server.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

Server::Server(int port, const string& password) : _password(password), _listeningPort(port), _listeningSocketFD(-1) {}

Server::~Server() {}

bool	Server::openSocket()
{
	_listeningSocketFD = socket(AF_INET /*IPv4 Internet protocols*/, SOCK_STREAM /*TCP*/, 0 /*default = TCP*/);

	if (_listeningSocketFD == SYSCALL_ERROR) {
		cerr << "Socket failed - Error : " << strerror(errno) << endl;
		return false;
	}
	cout << "Socket FD is " << _listeningSocketFD << endl;
	return true;
}

bool	Server::bindSocket() {
	memset(&_serverSocket, 0, sizeof(_serverSocket));	// To avoid garbage values

	_serverSocket.sin_family = AF_INET;
	_serverSocket.sin_port = htons(_listeningPort);	// htons() converts the unsigned short integer hostshort from host byte order to network byte order
	_serverSocket.sin_addr.s_addr = htons(INADDR_ANY);

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

// TEST COMMAND FROM TERMINAL : echo "Salut Poulette !" | nc 127.0.0.1 55555
void	Server::pollAndAccept()
{
	initAddPollFdStruct(_listeningSocketFD);

	while (SERVER_RUNNING)	// [TO DO] switch to false when signal received, to quit the program properly
	{
		if (poll(&_AllPollFDStructs[0], static_cast<nfds_t>(_AllPollFDStructs.size()), -1) == SYSCALL_ERROR) // Man poll : "Specifying a negative value in timeout means an infinite timeout"
		{
			cerr << "poll failed - Error : " << strerror(errno) << endl;
		}
		else
		{
			cout << "Poll Success ! Listening on : " << _AllPollFDStructs[0].fd << "\n";
			cout << "Output r-events : [" << _AllPollFDStructs[0].revents << "]\n";	// POLLHUP - Check s'il y a un revent qui n'est pas == 0 - Faire masque avec POLLIN (data to read)
			socklen_t addrlen = sizeof(_serverSocket);
			int	ClientSocket = accept(_AllPollFDStructs[0].fd, (struct sockaddr*) &_serverSocket, &addrlen);	// args : int sockfd, struct sockaddr *addr, socklen_t *addrlen)
			// CAUTION - accept() is blocking - Find a way that other connections / incoming messages are possible
			if (ClientSocket == -1) {
				cerr << "Client socket creation failed : " << strerror(errno) << endl;
			}
			else {
				// Add last ClientSocket in _AllPollFDStructs :
				initAddPollFdStruct(ClientSocket);
				// TO DO : Use & setup client class once we have all the info from client
			}
			// TO DO : Ask password, make sure it matches - Do that when everything else is working
			// Password accepted = Communication can start
			cout << "*** DEBUG ***\n";
			char buffer[20] = {0};
			read(ClientSocket, buffer, 19);			// TESTTING / DEBUG ONLY - Delete when everything is functional
			cout << buffer << endl;
			// If client sends something else, it doesn't print because server is waiting for a new connection - Fix that
		}
	}
}

// _listeningSocket is at position [0] or (_AllPollFDStructs.begin())
void	Server::initAddPollFdStruct(int socketFDToAdd)
{
	pollfd NewSocket;
	memset(&NewSocket, 0, sizeof(NewSocket));
	NewSocket.fd = socketFDToAdd;	// FD I want to monitor
	NewSocket.events = POLLIN ;		// = What is expected from this pollfd - Check full list in man poll() or poll.h - If many needed, separate with a pipe sign
	// NewSocket.revents = ;		// No need - Will be filled up automatically later. Read from it when necessary.

	_AllPollFDStructs.push_back(NewSocket);
}

void	Server::closeBeforeExit() {
	close(_listeningSocketFD);
	// Close all opened sockets & FDs
}
