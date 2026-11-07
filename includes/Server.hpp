#ifndef SERVER_HPP
#define SERVER_HPP

#include <netinet/in.h>  // sockaddr_in
#include <poll.h>        // pollfd

#include <map>
#include <string>  // std::string_password
#include <vector>

#include "Client.hpp"

#define SYSCALL_ERROR -1
#define SERVER_RUNNING true

class Server {
private:
    // Keep them private, otherwise anybody can copy a Server by accident.
    Server(const Server& other);
    Server& operator=(const Server& other);

    std::string _password;
    int _listeningPort;
    int _listeningSocketFD;
    // Cast to (struct sockaddr *) before functions that requires that struct
    sockaddr_in _serverSocket;
    // _listeningSocket is at position [0] or (.begin())
    std::vector<pollfd> _pollfdParty;
    // int is FD - Auto sort in alpha order
    std::map<int, Client> _clientsParty;

public:
    Server(int port, const std::string& password);
    ~Server();

    bool openSocket();
    bool bindSocket();
    bool listenSetUp();
    void pollAndAccept();
    void acceptNewClient();
    std::string convClientIP(const in_addr rawClientIP);
    bool isPasswordAccepted();
    void setUpNewSocket(int socketFDToAdd);
    void parsePollfds_n_receive();
    std::vector<pollfd>::iterator receive_n_sendToParsing(
        int ClientSocket, std::vector<pollfd>::iterator currentPos);
    void checkString(std::string& clientInBuffer);
    // TO DO - Close all opened sockets + FD before leaving
    void closeBeforeExit();
};

#endif
