#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/types.h>		// socket()
#include <sys/socket.h>		// socket()
#include <sys/un.h>			// strerror()
#include <arpa/inet.h>		// htons()
#include <cerrno>			// errno()
#include <stdlib.h>			// strtol
#include <unistd.h>			// close
#include <poll.h>			// poll
#include <vector>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

#define SYSCALL_ERROR -1
#define SERVER_RUNNING true

class Server
{
	private:
		Server(const Server &other);					// If they are public, anybody can copy a Server by accident.
		Server &operator=(const Server &other);			// If they are public, anybody can copy a Server by accident.

		string		_password;
		int			_listeningPort;
		int			_listeningSocketFD;
		// sockaddr structs man : https://man7.org/linux/man-pages/man7/unix.7.html
		sockaddr_in	_serverSocket;		// Cast to (struct sockaddr *) before functions that requires that struct
		// std::vector<Clients>;		// Comment out when ready to use
		// pollfd		_listeningSocket;
		std::vector<pollfd> _AllPollFDStructs;	// _listeningSocket is at position [0] or (.begin())
		// Buffer size 512 bits max (see Luca's error) + deal with max size
		// Luca's suggestion : take out the "\r\n" direct in the engine

	public:
		Server(int port, const string& password);
		~Server();

		bool	openSocket();
		bool	bindSocket();
		bool	listenSetUp();
		void	pollAndAccept();
		void	initAddPollFdStruct(int socketFDToAdd);
		void	closeBeforeExit();	// Close all opened sockets + FD before leaving
};

#endif
