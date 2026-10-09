#include <iostream>
#include <sys/types.h>		// socket()
#include <sys/socket.h>		// socket()
#include <arpa/inet.h>		// htons
#include <cerrno>			// errno
#include <stdlib.h>			// strtol
#include <sys/un.h>			// strerror
#include <unistd.h>			// close
#include <poll.h>			// poll

// define the port the server is listening on ? #define SERVER_PORT 00 or only for client side ?
#define INVALID_SOCKET -1

using namespace std;

/*
Your executable will be run as follows:
./ircserv <port> <password>
• port: The port number on which your IRC server will be listening for incoming IRC connections.
• password: The connection password. It will be needed by any IRC client that tries to connect to your server.
*/

int	main(int argc, char *argv[])
{
	/* if args != 3, return error msg w/ instructions */

	char *endptr;
	int listeningPort = static_cast<int>(strtol(argv[1], &endptr, 10));
	// check that port is between 1 & 65535

	// char *password = argv[2];	// For later

	int socketFileDesc = socket(AF_INET /*IPv4 Internet protocols*/, SOCK_STREAM, 0);
	/* About arg 2 : use for TCP - Provides sequenced, reliable, two-way, connection-based byte streams.

	About arg 3 : 0 = default = TCP - Man : Normally only a single protocol exists to support a particular socket type within a given  protocol family,
	in which case protocol can be specified as 0.  However, it is possible that many protocols may exist, in which case a particular protocol must be specified
	in this manner.  The protocol number to use is specific to the “communication domain” in which communication is to take place; see protocols(5).  See  getprotoent(3)  on
	how to map protocol name strings to protocol numbers.*/

	// Create infinite loop here ? To accept every client connecting, or is that the job of poll()&co ?

	if (socketFileDesc == INVALID_SOCKET)
	{
		cout << "Invalid Socket - Error : " << strerror(errno) << endl;
		return 1;
	}
	else
		cout << "Socket FD is " << socketFileDesc << endl;

	sockaddr_in serverSocket;		// different variables in struct sockaddr_un or sockaddr_in - If used, cast to (struct sockaddr *) before functions that requires that struct

// READ MAN : https://man7.org/linux/man-pages/man7/unix.7.html
// All answers about sockaddr structs might be there - Search Youtube also

	// Reinitialiser struct car elle peut avoir des valeurs aleatoires
	memset(&serverSocket, 0, sizeof(serverSocket));

	serverSocket.sin_family = AF_INET;
	serverSocket.sin_port = htons(listeningPort);	// htons() converts the unsigned short integer hostshort from host byte order to network byte order
	serverSocket.sin_addr.s_addr = INADDR_ANY;

	if (bind(socketFileDesc, reinterpret_cast<sockaddr*>(&serverSocket), sizeof(serverSocket)) != 0)	// Try c++ cast instead
	{
		cout << "Binding failed - Error : " << strerror(errno) << endl;
		close(socketFileDesc);
		return 1;
	}
	else
		cout << "Binding Success !\n";

	listen(socketFileDesc, 3/* Max connections we want to authorize on this socket*/);
	// Add fail condition

	pollfd new_Pollfd_Struct;
	memset(&new_Pollfd_Struct, 0, sizeof(new_Pollfd_Struct));
	new_Pollfd_Struct.fd = socketFileDesc;	// FD I want to monitor
	new_Pollfd_Struct.events = POLLIN ;		// Not sure - Check full list in man poll() - If many needed, separate with a pipe sign
	// new_Pollfd_Struct.revents = ;		// No need ?

	while (true)
	{
		if (poll(&new_Pollfd_Struct.fd, 1 /*amount of elements to analyse ?*/, 100) == -1)
		{
			cout << "poll failed - Error : " << strerror(errno) << endl;
		}

		socklen_t addrlen = sizeof(&serverSocket);	// Not sure
		int	ClientSocket = accept(socketFileDesc, (struct sockaddr*) &serverSocket, &addrlen);	// args : int sockfd, struct sockaddr *addr, socklen_t *addrlen)
		// Add fail condition

	}
	// Close all opened sockets before leaving
	close(socketFileDesc);
	return 0;
}
