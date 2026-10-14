#ifndef SERVER_HPP
#define SERVER_HPP

class Server
{
	private:
		int	_listeningPort;
		int	_listeningSocket;
		char _password[];		// Char* was too easy to access from the source code if stored that way - Option for a solution : store as a char array
		// Nooooo it's not what we thought, password is provided as an arg, it's NOT a verification - So keep as a char*
		/* Container of clients ? W/ sockets from Server + from clients + buffers */
	public:
		Server(/* args */);
		Server(Server &other);
		Server &operator=(Server &other);
		~Server();
};

Server::Server(/* args */)
{
	// set pw here with argv[2] (in init. list)
}

Server::~Server()
{
}



#endif
