#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"

#include <vector>
#include <map>
#include <poll.h>			// pollfd
#include <string>			// std::string_password
#include <netinet/in.h>  	// sockaddr_in

#define SYSCALL_ERROR -1
#define SERVER_RUNNING true

class Server
{
	private:
		Server(const Server &other);					// If they are public, anybody can copy a Server by accident.
		Server &operator=(const Server &other);			// If they are public, anybody can copy a Server by accident.

		std::string		_password;
		int			_listeningPort;
		int			_listeningSocketFD;
		sockaddr_in	_serverSocket;		// Cast to (struct sockaddr *) before functions that requires that struct
		std::vector<pollfd>		_pollfdParty;	// _listeningSocket is at position [0] or (.begin())
		std::map<int, Client>	_clientsParty;	// int is FD - Auto sort in alpha order
		// Buffer size 512 bits max (see Luca's error) + deal with max size
		// Luca's suggestion : take out the "\r\n" direct in the engine

	public:
		Server(int port, const std::string& password);
		~Server();

		bool	openSocket();
		bool	bindSocket();
		bool	listenSetUp();
		void	pollAndAccept();
		void	acceptNewClient();
		bool	isPasswordAccepted();
		void	setUpNewSocket(int socketFDToAdd);
		void	parsePollfds_n_receive();
		std::vector<pollfd>::iterator	receive_n_sendToParsing(int	ClientSocket, std::vector<pollfd>::iterator currentPos);
		void	checkString(std::string &clientBuffer);
		void	closeBeforeExit();	// Close all opened sockets + FD before leaving
};

#endif
