#include "Client.hpp"
#include "Server.hpp"
#include "colors.hpp"

#include <cerrno>		// errno
#include <iostream>		// cout, etc.
#include <unistd.h>		// close
#include <arpa/inet.h>	// htons, htonl
#include <cstring>		// memset, strerror
#include <fcntl.h>		// O_NONBLOCK, etc.
#include <sys/socket.h>	// socket, accept, etc.

Client::Client(int fd) : _fd(fd) {}

Client::~Client() {}

std::string&	Client::getInBuffer()
{
	return _inBuffer;
}
