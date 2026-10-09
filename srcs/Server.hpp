#ifndef SERVER_HPP
#define SERVER_HPP

class Server
{
	private:
		int	_listeningPort;
		int	_listeningSocket;
		char *_password;
		/* Container of clients ? W/ sockets from Server + from clients + buffers */
	public:
		Server(/* args */);
		Server(Server &other);
		Server &operator=(Server &other);
		~Server();
};

Server::Server(/* args */)
{
}

Server::~Server()
{
}


#endif
