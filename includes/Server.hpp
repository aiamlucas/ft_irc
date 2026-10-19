#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/types.h>		// socket()
#include <sys/socket.h>		// socket()
#include <sys/un.h>			// strerror()
#include <arpa/inet.h>		// htonl()
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
		string	_password;
		int		_listeningPort;
		int		_listeningSocketFD;
		// sockaddr structs man : https://man7.org/linux/man-pages/man7/unix.7.html
		sockaddr_in	_serverSocket;		// struct sockaddr_un or sockaddr_in ? Either way, cast to (struct sockaddr *) before functions that requires that struct
		// std::vector<Clients>;		// Comment out when ready to use

	public:
		Server(int port, const string& password);
		Server(Server &other);					// TBC - Prob not required
		Server &operator=(Server &other);		// TBC - Prob not required
		~Server();

		bool	openSocket();
		bool	bindSocket();
		bool	listenSetUp();
		void	pollAndAccept();
		void	closeBeforeExit();	// Close all opened sockets + FD before leaving
};

#endif
