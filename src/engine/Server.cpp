#include "Server.hpp"

#include <arpa/inet.h>   // htons()
#include <fcntl.h>       // fcntl
#include <stdlib.h>      // strtol
#include <sys/socket.h>  // socket()
#include <sys/types.h>   // socket()
#include <sys/un.h>      // strerror()
#include <unistd.h>      // close

#include <cerrno>  // errno()
#include <iostream>

#include "colors.hpp"

/*
Next : function to remove clients from everything
+ buffer with what's following \r\n - No clue where to put it because
the buffer is common to all clients. Add a temp buffer in client ?
*/

#define MAX_BUFFER_SIZE 512
using std::cerr;
using std::cout;
using std::endl;
using std::string;

Server::Server(int port, const string& password)
    : _password(password), _listeningPort(port), _listeningSocketFD(-1) {}

Server::~Server() {}

bool Server::openSocket() {
    if ((_listeningSocketFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        cerr << CL_RED
             << "Issue w/ listening socket creation : " << strerror(errno)
             << " on listening socket fd : " << _listeningSocketFD << RESET
             << endl;

    // Only args allowed by subject
    if (fcntl(_listeningSocketFD, F_SETFL, O_NONBLOCK) == -1)
        cerr << CL_RED << "Issue w/ fcntl() : " << strerror(errno)
             << " on listening socket fd : " << _listeningSocketFD << RESET
             << endl;

    if (_listeningSocketFD == SYSCALL_ERROR) {
        cerr << CL_RED << "Socket failed - Error : " << strerror(errno) << RESET
             << endl;
        return false;
    }
    cout << "Listening FD is " << _listeningSocketFD << endl;
    return true;
}

bool Server::bindSocket() {
    // To avoid garbage values
    memset(&_serverSocket, 0, sizeof(_serverSocket));

    // Specifies we'll be using a IPv4 address
    _serverSocket.sin_family = AF_INET;
    // Network port - FYI htons() converts the unsigned short integer hostshort
    // from host byte order to the byte order used by network
    // Note for Lucas : htons (rather than htonl) must always be used for ports,
    // because ports are always 16 bits
    _serverSocket.sin_port = htons(_listeningPort);
    // IPv4 address - Here, every bit is 0 (INADDR_ANY is 0) - htonl() used
    // because IP is 32 bits
    // INADDR_ANY is a specific address that means that the server listens to
    // all the local IPv4 compatible with this socket
    _serverSocket.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(_listeningSocketFD, reinterpret_cast<sockaddr*>(&_serverSocket),
             sizeof(_serverSocket)) != 0) {
        cerr << CL_RED << "Binding failed - Error : " << strerror(errno)
             << RESET << endl;
        close(_listeningSocketFD);
        return false;
    }
    cout << "Binding Success !\n";
    return true;
}

bool Server::listenSetUp() {
    // 4 = Size of queue for incoming connexions (not accepted yet)
    if (listen(_listeningSocketFD, 4) != 0) {
        cerr << CL_RED << "Listening failed - Error : " << strerror(errno)
             << RESET << endl;
        close(_listeningSocketFD);
        return false;
    }
    cout << "Listening ...\n";
    return true;
}

// TEST COMMAND FROM TERMINAL : echo "Salut Poulette !" | nc 127.0.0.1 55555
void Server::pollAndAccept() {
    setUpNewSocket(_listeningSocketFD);

    // [TO DO] switch to false when signal received, to quit the program
    // properly
    while (SERVER_RUNNING) {
        // Man poll : "negative value in timeout means an infinite timeout"
        if (poll(&_pollfdParty[0], static_cast<nfds_t>(_pollfdParty.size()),
                 -1) == SYSCALL_ERROR) {
            cerr << CL_RED << "poll failed - Error : " << strerror(errno)
                 << RESET << endl;
        } else {
            // poll.h : #define POLLIN		0x001 = There is data to read
            if (_pollfdParty[0].revents & POLLIN)
            // Do I need to check for revents other than POLLIN ?
            // if (_pollfdParty[0].revents != 0)
            {
                // cout << "Poll Success ! Listening on fd : " <<
                // _pollfdParty[0].fd << "\n"; cout << "Output r-events : [" <<
                // _pollfdParty[0].revents << "]\n";
                // POLLHUP - Check s'il y a un revent qui n'est pas == 0
                acceptNewClient();
            }
        }
        parsePollfds_n_receive();
    }
}

void Server::acceptNewClient() {
    sockaddr_in newClientAddr;
    memset(&newClientAddr, 0, sizeof(sockaddr_in));
    socklen_t addrlen = sizeof(newClientAddr);
    int ClientSocket =
        accept(_pollfdParty[0].fd, (struct sockaddr*)&newClientAddr, &addrlen);
    cout << CL_LIGHT_BLUE << "Client port is : " << newClientAddr.sin_port
         << "\nClient IP is : " << newClientAddr.sin_addr.s_addr << RESET
         << endl;
    // Only args allowed by subject
    if (fcntl(ClientSocket, F_SETFL, O_NONBLOCK) == -1)
        cerr << CL_RED << "Issue w/ fcntl() : " << strerror(errno)
             << " on client fd : " << ClientSocket << RESET << endl;

    if (ClientSocket == -1) {
        cerr << CL_RED << "Client socket creation failed : " << strerror(errno)
             << RESET << endl;
    } else {
        // Add last ClientSocket in _pollfdParty + _clientsParty :
        setUpNewSocket(ClientSocket);
        Client NewClient(ClientSocket);
        // Add Client IP here
        _clientsParty.insert(std::pair<int, Client>(ClientSocket, NewClient));
    }
}
//     addClientIP();
void Server::addClientIP() {}

void Server::parsePollfds_n_receive() {
    for (std::vector<pollfd>::iterator it = _pollfdParty.begin();
         it != _pollfdParty.end(); it++) {
        size_t vecSiz = _pollfdParty.size();
        if ((*it).fd == _listeningSocketFD) continue;
        if ((*it).revents & POLLIN) it = receive_n_sendToParsing((*it).fd, it);
        // POLLERR = Error condition	POLLHUP = Hung up (Full list in poll.h)
        else if (((*it).revents & POLLERR) || ((*it).revents & POLLHUP)) {
            cout << CL_RED_REVERT << "Debug - revents is not POLLIN : ["
                 << "]\n"
                 << RESET;
            close((*it).fd);
        }
        if (vecSiz != _pollfdParty.size()) it--;
    }
}

std::vector<pollfd>::iterator Server::receive_n_sendToParsing(
    int ClientSocket, std::vector<pollfd>::iterator currentPos) {
    cout << CL_GREEN_BOLD << "*** BYTES INCOMING ! ***\n" << RESET;
    // !! A cmd can be splitted into many buffers
    // TBC Can 2 cmds be in the same buffer ?

    ssize_t bytesReceived = 0;
    char buffer[MAX_BUFFER_SIZE] = {0};
    // Flag MSG_DONTWAIT enables nonblocking operation but used fcntl() instead
    bytesReceived = recv(ClientSocket, &buffer, MAX_BUFFER_SIZE, 0);

    // If client sends something else, it doesn't print because server is
    // waiting for a new connection - Fix that
    if (bytesReceived == -1)
    // Subject : After recv(),you must not rely on errno to decide what to do
    // next (for instance, reading again after errno == EAGAIN). Doing so will
    // set your grade to 0.
    {
        cerr << CL_BLUE
             << "-1 bytes received - Nothing to read or issue ? Investigate. "
                "Tmp Debug Errno (forbidden - to remove) : "
             << strerror(errno) << RESET << endl;
        return currentPos;
    } else if (bytesReceived == 0) {
        cout << CL_YELLOW << "Closing FD\n --------" << RESET << endl;
        _clientsParty.erase(ClientSocket);
        close(ClientSocket);
        // Erase returns() an iterator to the following element
        return (_pollfdParty.erase(currentPos));
    }

    std::map<int, Client>::iterator SearchclientInBuffer =
        _clientsParty.find(ClientSocket);
    // TO DO : edge case if find returns end (very low prob but good practice)
    string& clientInBuffer = (*SearchclientInBuffer).second.getInBuffer();

    clientInBuffer.append(buffer, bytesReceived);

    if (clientInBuffer.find('\n' /*, sizeof(clientInBuffer)*/) ==
        clientInBuffer.npos) {
        cout << CL_PURPLE_DIM
             << "Command cropped (additional bytes incoming ...)\n"
             << RESET;
    } else {
        // Check if chars after line return and store them in buffer ?
        cout << CL_BLUE_DIM
             << "Line return found - String might good to be handled by "
                "parsing - Checking ...\n"
             << RESET;
        checkString(clientInBuffer);
    }

    cout << "Buffer content : -->" << string(buffer, bytesReceived) << "<-- ("
         << bytesReceived << " bytes received)"
         << " - Client inBuffer : [" << clientInBuffer << "] From FD "
         << ClientSocket << "\n --------------\n";

    return currentPos;
}

/*
NB / TODO : One read can contain several lines,
or just a line
or half of the next line (similar logic to the ft_getnextline)
Skip empty lines
*/
void Server::checkString(string& clientInBuffer) {
    if (clientInBuffer.size() > 510) {
        cerr << CL_RED_BOLD
             << "\t... Buffer is too long - Max 510 chars allowed\n\n"
             << RESET;
        clientInBuffer.erase();
        return;
    }

    // save iterator/position for \n and for \r
    size_t CR = clientInBuffer.find_first_of('\r');
    size_t LF = clientInBuffer.find_first_of('\n');

    // Check if there is anything after the \r\n / \n
    if (CR != string::npos) {
        // Only remove \r\n, check if anything after, and store it in a buffer
        clientInBuffer.erase(CR, 2);
    } else if (LF != string::npos) {
        // !! Only remove \n, check if anything after, and store it in a buffer
        clientInBuffer.erase(LF);
    }

    cout << CL_GREEN_BOLD << "\t... GOOD TO GO !\n\n" << RESET;
}

// _listeningSocket is at position [0] or (_pollfdParty.begin())
void Server::setUpNewSocket(int socketFDToAdd) {
    pollfd NewSocket;
    memset(&NewSocket, 0, sizeof(NewSocket));
    // FD I want to monitor
    NewSocket.fd = socketFDToAdd;
    // = What is expected from this pollfd
    // Check full list in man poll() or poll.h
    // If many needed, separate with a pipe sign
    NewSocket.events = POLLIN;
    // No need - Will be filled up automatically later.
    // NewSocket.revents = ;

    _pollfdParty.push_back(NewSocket);
}

void Server::closeBeforeExit() {
    close(_listeningSocketFD);
    // Close all opened sockets & FDs
}
