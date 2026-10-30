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
#include <fcntl.h>			// fcntl
#include <vector>
#include <map>

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
		std::vector<pollfd>		_pollfdParty;	// _listeningSocket is at position [0] or (.begin())
		std::map<int, Client>	_clientsParty;	// int is FD - Auto sort in alpha order
		// Buffer size 512 bits max (see Luca's error) + deal with max size
		// Luca's suggestion : take out the "\r\n" direct in the engine

	public:
		Server(int port, const string& password);
		~Server();

		bool	openSocket();
		bool	bindSocket();
		bool	listenSetUp();
		void	pollAndAccept();
		void	acceptNewClient();
		// PW check function
		void	setUpNewSocket(int socketFDToAdd);
		void	parsePollfds_n_receive();
		std::vector<pollfd>::iterator	receive_n_sendToParsing(int	ClientSocket, std::vector<pollfd>::iterator currentPos);
		void	checkString(string &clientBuffer);
		void	closeBeforeExit();	// Close all opened sockets + FD before leaving
};

#endif
