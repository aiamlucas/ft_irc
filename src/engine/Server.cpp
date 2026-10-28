#include "../../includes/Server.hpp"
#include "../../includes/colors.hpp"

#define MAX_BUFFER_SIZE 512
#define TEST_MAX_BUFF_SIZ 20

using std::cout;
using std::cerr;
using std::endl;
using std::string;

Server::Server(int port, const string& password) : _password(password), _listeningPort(port), _listeningSocketFD(-1) {}

Server::~Server() {}

bool	Server::openSocket()
{
	_listeningSocketFD = socket(AF_INET /*IPv4 Internet protocols*/, SOCK_STREAM /*TCP*/, 0 /*default = TCP*/);
	if (fcntl(_listeningSocketFD, F_SETFL, O_NONBLOCK) == -1)			// Only args allowed by subject
		cerr << CL_RED << "Issue w/ fcntl() : " << strerror(errno) << " on listening socket fd : " << _listeningSocketFD << RESET << endl;

	if (_listeningSocketFD == SYSCALL_ERROR) {
		cerr << CL_RED << "Socket failed - Error : " << strerror(errno) << RESET << endl;
		return false;
	}
	cout << "Socket FD is " << _listeningSocketFD << endl;
	return true;
}

bool	Server::bindSocket() {
	memset(&_serverSocket, 0, sizeof(_serverSocket));	// To avoid garbage values

	_serverSocket.sin_family = AF_INET;					// Specify we'll be using a IPv4 address
	_serverSocket.sin_port = htons(_listeningPort);		// Network port - FYI htons() converts the unsigned short integer hostshort from host byte order to the byte order used by network
	// Note for Lucas : htons (rather than htonl) must always be used for ports, because ports are always 16 bits
	_serverSocket.sin_addr.s_addr = htonl(INADDR_ANY);	// IPv4 address - Here, every bit is 0 (INADDR_ANY is 0) - htonl() used because IP is 32 bits
	// INADDR_ANY is a specific address that means that the server listens to all the local IPv4 compatible with this socket

	if (bind(_listeningSocketFD, reinterpret_cast<sockaddr*>(&_serverSocket), sizeof(_serverSocket)) != 0)
	{
		cerr << CL_RED << "Binding failed - Error : " << strerror(errno) << RESET << endl;
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
		cerr << CL_RED << "Listening failed - Error : " << strerror(errno) << RESET << endl;
		close(_listeningSocketFD);
		return false;
	}
	cout << "Listening ...\n";
	return true;
}

// TEST COMMAND FROM TERMINAL : echo "Salut Poulette !" | nc 127.0.0.1 55555
void	Server::pollAndAccept()
{
	setUpNewSocket(_listeningSocketFD);

	while (SERVER_RUNNING)	// [TO DO] switch to false when signal received, to quit the program properly
	{
		if (poll(&_pollfdParty[0], static_cast<nfds_t>(_pollfdParty.size()), -1) == SYSCALL_ERROR) // Man poll : "Specifying a negative value in timeout means an infinite timeout"
		{
			cerr << CL_RED << "poll failed - Error : " << strerror(errno) << RESET << endl;
		}
		else
		{
			if (_pollfdParty[0].revents & POLLIN)	// poll.h : #define POLLIN		0x001		/* There is data to read.  */
			// if (_pollfdParty[0].revents != 0)		// Do I need to check for revents other than POLLIN ?
			{
				cout << "Poll Success ! Listening on fd : " << _pollfdParty[0].fd << "\n";
				cout << "Output r-events : [" << _pollfdParty[0].revents << "]\n";	// POLLHUP - Check s'il y a un revent qui n'est pas == 0 - Faire masque avec POLLIN (data to read)
				acceptNewClient();
			}
		}
		parsePollfds_n_receive();
	}
}

void	Server::acceptNewClient()
{
	// sockaddr_in newClientAddr = {0};					// Comment out if client address needed
	// socklen_t addrlen = sizeof(newClientAddr);		// Comment out if client address needed
	int	ClientSocket = accept(_pollfdParty[0].fd, NULL, NULL);	// if client address needed, replace this line by line below
	// int	ClientSocket = accept(_pollfdParty[0].fd, (struct sockaddr*) &newClientAddr, &addrlen);	// args : int sockfd, struct sockaddr *addr, socklen_t *addrlen)
	if (fcntl(ClientSocket, F_SETFL, O_NONBLOCK) == -1)			// Only args allowed by subject
		cerr << CL_RED << "Issue w/ fcntl() : " << strerror(errno) << " on client fd : " << ClientSocket << RESET << endl;

	if (ClientSocket == -1) {
		cerr << CL_RED << "Client socket creation failed : " << strerror(errno) << RESET << endl;
	}
	else {
		// Add last ClientSocket in _pollfdParty + _clientsParty :
		setUpNewSocket(ClientSocket);
		Client NewClient(ClientSocket);
		_clientsParty.insert(std::pair<int, Client>(ClientSocket, NewClient));
		// TO DO : Ask password, make sure it matches - Do that when everything else is working - Password accepted = Communication can start
	}
}

void	Server::parsePollfds_n_receive()
{
	for (std::vector<pollfd>::iterator it = _pollfdParty.begin(); it != _pollfdParty.end(); it++)
	{
		size_t vecSiz = _pollfdParty.size();
		if ((*it).fd == _listeningSocketFD)
			continue;
		if ((*it).revents & POLLIN)
			it = receive_n_sendToParsing((*it).fd, it);
		if (vecSiz != _pollfdParty.size())
			it--;
	}
}

std::vector<pollfd>::iterator	Server::receive_n_sendToParsing(int	ClientSocket, std::vector<pollfd>::iterator currentPos)
{
	cout << CL_GREEN_BOLD << "*** BYTES INCOMING ! ***\n" << RESET;
	// !! A cmd can be splitted into many buffers
	// TBC Can 2 cmds be in the same buffer ?

	ssize_t bytesReceived = 0;
	std::map<int, Client>::iterator SearchClientBuffer = _clientsParty.find(ClientSocket);

// =============== START HERE / FIX THAT ==================== //

	char *buffer = (*SearchClientBuffer).second.getInBuffer();
	// char buffer[5] = {0};	// \n is automatically added in the buffer even if no line break happens (nc cmd)
	bytesReceived = recv(ClientSocket, &buffer, 5, 0);	// FYI : Flag MSG_DONTWAIT enables nonblocking operation but used fcntl() instead

	// If client sends something else, it doesn't print because server is waiting for a new connection - Fix that
	if (bytesReceived == -1)
	// Subject : After recv(),you must not rely on errno to decide what to do next (for instance, reading again after errno == EAGAIN). Doing so will set your grade to 0.
	{
		cerr << CL_BLUE << "-1 bytes received - Nothing to read or issue ? Investigate. Tmp Debug Errno (forbidden - to remove) : " << strerror(errno) << RESET << endl;
		return currentPos;
	}
	else if (bytesReceived == 0)
	{
		cout << CL_YELLOW << "Closing FD\n --------" << RESET << endl;
		_clientsParty.erase(ClientSocket);
		close(ClientSocket);
		return (_pollfdParty.erase(currentPos));		// Erase returns() an iterator to the following element
	}

	std::string bufferStack;
	bufferStack.append(buffer);

	if ((buffer[bytesReceived - 1]) != '\n')
	{
		cerr << CL_RED << "Command cropped (additional bytes incoming ...)\n" << RESET;
		cout << "Buffer content : [" << buffer << "] - BuffStack : [" << bufferStack << "] From FD " << ClientSocket << " (" << bytesReceived << " bytes received)" << endl;
		// the rest of the bits will still be received anyway after that
	}
	else
	{
		// \n found, check if chars after that and send to parsing
		cout << "Buffer content : [" << buffer << "] - BuffStack : [" << bufferStack << "] From FD " << ClientSocket << " (" << bytesReceived << " bytes received)" << endl;
	}
	cout << " --------------\n";
	return currentPos;
}

// _listeningSocket is at position [0] or (_pollfdParty.begin())
void	Server::setUpNewSocket(int socketFDToAdd)
{
	pollfd NewSocket;
	memset(&NewSocket, 0, sizeof(NewSocket));
	NewSocket.fd = socketFDToAdd;	// FD I want to monitor
	NewSocket.events = POLLIN ;		// = What is expected from this pollfd - Check full list in man poll() or poll.h - If many needed, separate with a pipe sign
	// NewSocket.revents = ;		// No need - Will be filled up automatically later. Read from it when necessary.

	_pollfdParty.push_back(NewSocket);
}

void	Server::closeBeforeExit() {
	close(_listeningSocketFD);
	// Close all opened sockets & FDs
}
