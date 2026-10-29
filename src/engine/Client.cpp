#include "../../includes/Client.hpp"

Client::Client(int fd) : _fd(fd) {}

Client::~Client() {}

std::string&	Client::getInBuffer()
{
	return _inBuffer;
}
