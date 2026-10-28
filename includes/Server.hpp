#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"

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
		sockaddr_in	_serverSocket;		// Cast to (struct sockaddr *) before functions that requires that struct
		std::vector<Client>	_AllClientsStructs;
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
		void	setUpNewSocket(int socketFDToAdd);
		std::vector<pollfd>::iterator	receive_n_sendToParsing(int	ClientSocket, std::vector<pollfd>::iterator currentPos);
		// PW check function
		void	closeBeforeExit();	// Close all opened sockets + FD before leaving
};

#endif
