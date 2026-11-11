#ifndef HANDLERS_HPP
#define HANDLERS_HPP

#include <string>

class Client;  // forward declaration
class Server;  // forward declaration

// handle a line recieved from the client without "\r\n"
void handleLine(Server& server, Client& client, const std::string& line);

#endif
