#include <cstdlib>
#include <iostream>
#include <string>

#include "Client.hpp"
#include "Handlers.hpp"
#include "colors.hpp"

// empty server, so that we can call handleLine in the tests
// (no need to open sockets, etc)
class Server {};

static int g_total = 0;
static int g_failed = 0;

static void expectHandleLine(Client& client, const std::string& line,
                             const std::string& expected) {
    Server server;
    handleLine(server, client, line);
    std::string output = client.getOutBuffer();
    client.eraseOutBuffer(output.size());
    g_total++;
    if (output == expected) {
        std::cout << "\n"
                  << CL_GREEN << "OK: " << RESET << "\"" << line << "\""
                  << "\n"
                  << output << std::endl;
        return;
    }
    g_failed++;
    std::cerr << CL_RED << "FAIL: " << RESET << "\"" << line << "\"\n"
              << " expected: " << expected << "\n"
              << " output:    " << output << std::endl;
}

// commands tested: PING
// commands partly tested: NICK and JOIN (only the missing parameter)
// TODO: test NICK and JOIN when they are fully implemented
int main() {
    std::string line;
    std::string expected;

    // Test 1
    // PING: shoud answer with PONG and the same token
    {
        Client client(4);
        line = "PING oie";
        expected = ":oipoulette PONG oipoulette :oie\r\n";
        expectHandleLine(client, line, expected);
    }
    // Test 2
    // PING again but this time lowercase (it should work)
    {
        Client client(4);
        line = "ping oie";
        expected = ":oipoulette PONG oipoulette :oie\r\n";
        expectHandleLine(client, line, expected);
    }
    // Test 3
    // PING without a token (should be 461 not enough parameters)
    {
        Client client(4);
        line = "PING";
        expected = ":oipoulette 461 * PING :Not enough parameters\r\n";
        expectHandleLine(client, line, expected);
    }
    // Test 4
    // NICK without a nickname (431 no nickname given)
    {
        Client client(4);
        line = "NICK";
        expected = ":oipoulette 431 * :No nickname given\r\n";
        expectHandleLine(client, line, expected);
    }
    // Test 5
    // JOIN (without a channel -> should give 461)
    {
        Client client(4);
        line = "JOIN";
        expected = ":oipoulette 461 * JOIN :Not enough parameters\r\n";
        expectHandleLine(client, line, expected);
    }
    // Test 6
    // NOHANDLER (command is not in the lookup list -> should give 421)
    {
        Client client(4);
        line = "NOHANDLER";
        expected = ":oipoulette 421 * NOHANDLER :Unknown command\r\n";
        expectHandleLine(client, line, expected);
    }
    // Test 7
    // two commands in a row from the same client
    {
        Client client(4);
        line = "PING oie";
        expected = ":oipoulette PONG oipoulette :oie\r\n";
        expectHandleLine(client, line, expected);

        line = "PING hi";
        expected = ":oipoulette PONG oipoulette :hi\r\n";
        expectHandleLine(client, line, expected);
    }

    std::cout << "\n"
              << (g_total - g_failed) << "/" << g_total << " tests passed"
              << std::endl;

    if (g_failed) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
