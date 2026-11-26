#include <cstdlib>
#include <iostream>
#include <string>

#include "Client.hpp"
#include "Handlers.hpp"
#include "IrcMacros.hpp"
#include "Server.hpp"
#include "colors.hpp"

// no sockets are opened, the password is "poulettepass"
static Server server(6667, "poulettepass");

static int g_total = 0;
static int g_failed = 0;

static void printTest(const std::string& title) {
    std::cout << "\n" << CL_GREY << "\\\\" << title << RESET << std::endl;
}

static void expectHandleLine(Client& client, const std::string& line,
                             const std::string& expected) {
    handleLine(server, client, line);
    std::string output = client.getOutBuffer();
    client.eraseOutBuffer(output.size());
    g_total++;
    if (output == expected) {
        std::cout << CL_GREEN << "OK: " << RESET << "\"" << line << "\""
                  << "\n"
                  << output;
        return;
    }
    g_failed++;
    std::cerr << CL_RED << "FAIL: " << RESET << "\"" << line << "\"\n"
              << " expected: " << expected << "\n"
              << " output:    " << output << std::endl;
}

static void expectPasswordOk(Client& client, bool expected) {
    g_total++;
    if (client.isPasswordOk() == expected) {
        std::cout << BOLD << "Password accepted: " << std::boolalpha << expected
                  << RESET << std::endl;
        return;
    }
    g_failed++;
    std::cerr << CL_RED << "FAIL: " << RESET BOLD << "Password accepted:\n"
              << std::boolalpha << " expected: " << expected << "\n"
              << " got: " << client.isPasswordOk() << RESET << std::endl;
}

static void expectUsername(const Client& client, const std::string& expected) {
    g_total++;
    if (client.getUsername() == expected) {
        std::cout << BOLD << "Username:: " << expected << RESET << std::endl;
        return;
    }
    g_failed++;
    std::cerr << CL_RED << "FAIL: " << RESET BOLD << "Username:\n"
              << " expected: " << expected << "\n"
              << " got: " << client.getUsername() << RESET << std::endl;
}

// commands partly tested: NICK and JOIN (only the missing parameter)
// TODO: test NICK and JOIN when they are fully implemented
int main() {
    std::string line;
    std::string expected;

    // The welcome block: the server sends these five lines when
    // the registration is complete:
    //    :<server> <code> <nick> <text>
    // 001 tells the client that it's registered (WeeChat reads the nickname
    // from it). 002-004 describe the server, 422 says there's no MOTD.
    // Ex: nick "ssin" username "ssin" host "127.0.0.1"
    // if the date in the 003 line differs, do make re (date is set to the
    // compiled date)
    const std::string WELCOME =
        ":" SERVER_NAME " " RPL_WELCOME " ssin " RPL_WELCOME_MSG
        "ssin!ssin@127.0.0.1\r\n"
        ":" SERVER_NAME " " RPL_YOURHOST " ssin " RPL_YOURHOST_MSG
        "\r\n"
        ":" SERVER_NAME " " RPL_CREATED " ssin " RPL_CREATED_MSG
        "\r\n"
        ":" SERVER_NAME " " RPL_MYINFO " ssin " RPL_MYINFO_MSG
        "\r\n"
        ":" SERVER_NAME " " ERR_NOMOTD " ssin " ERR_NOMOTD_MSG "\r\n";

    {
        printTest("Test 1: PING: should answer with PONG and the same token");
        Client client(4);
        line = "PING oie";
        expected = ":oipoulette PONG oipoulette :oie\r\n";
        expectHandleLine(client, line, expected);
    }
    {
        printTest(
            "Test 2: PING in lowercase (should answer with PONG and same "
            "token)");
        Client client(4);
        line = "ping oie";
        expected = ":oipoulette PONG oipoulette :oie\r\n";
        expectHandleLine(client, line, expected);
    }
    {
        printTest("Test 3: PING without a token (461 not enough parameters)");
        Client client(4);
        line = "PING";
        expected = ":oipoulette 461 * PING :Not enough parameters\r\n";
        expectHandleLine(client, line, expected);
    }
    {
        printTest("Test 4: NICK without a nickname (431 no nickname given)");
        Client client(4);
        line = "NICK";
        expected = ":oipoulette 431 * :No nickname given\r\n";
        expectHandleLine(client, line, expected);
    }
    {
        printTest("Test 5: JOIN without a channel (461 not enough parameters)");
        Client client(4);
        line = "JOIN";
        expected = ":oipoulette 461 * JOIN :Not enough parameters\r\n";
        expectHandleLine(client, line, expected);
    }
    {
        printTest("Test 6: NOHANDLER, not in the lookup table (421)");
        Client client(4);
        line = "NOHANDLER";
        expected = ":oipoulette 421 * NOHANDLER :Unknown command\r\n";
        expectHandleLine(client, line, expected);
    }
    {
        printTest("Test 7: two commands in a row from the same client");
        Client client(4);
        line = "PING oie";
        expected = ":oipoulette PONG oipoulette :oie\r\n";
        expectHandleLine(client, line, expected);

        line = "PING hi";
        expected = ":oipoulette PONG oipoulette :hi\r\n";
        expectHandleLine(client, line, expected);
    }
    {
        printTest("Test 8: PASS without a parameter (461, not accepted)");
        Client client(4);
        line = "PASS";
        expected = ":oipoulette 461 * PASS :Not enough parameters\r\n";
        expectHandleLine(client, line, expected);
        expectPasswordOk(client, false);
    }
    {
        printTest(
            "Test 9: PASS with the wrong password (464, password not "
            "accepted)");
        Client client(4);
        line = "PASS thispassinsnotcorrect";
        expected = ":oipoulette 464 * :Password incorrect\r\n";
        expectHandleLine(client, line, expected);
        expectPasswordOk(client, false);
    }
    {
        printTest("Test 10: PASS with the right password (no reply, accepted)");
        Client client(4);
        line = "PASS poulettepass";
        expected = "";
        expectHandleLine(client, line, expected);
        expectPasswordOk(client, true);
    }
    {
        printTest("Test 11: PASS when the client is already registered (462)");
        Client client(4);
        client.setPasswordOk();
        client.setNickname("ssin");
        client.setUsername("ssin");
        line = "PASS poulettepass";
        expected = ":oipoulette 462 ssin :You may not reregister\r\n";
        expectHandleLine(client, line, expected);
    }
    {
        printTest("Test 12: USER without enough parameters (461)");
        Client client(4);
        line = "USER ssin";
        expected = ":oipoulette 461 * USER :Not enough parameters\r\n";
        expectHandleLine(client, line, expected);
    }
    {
        printTest("Test 13: USER when the client is registered");
        Client client(4);
        client.setPasswordOk();
        client.setNickname("ssin");
        client.setUsername("ssin");
        line = "USER ssin 0 * :Sara Sin";
        expected = ":oipoulette 462 ssin :You may not reregister\r\n";
        expectHandleLine(client, line, expected);
        expectUsername(client, "ssin");
    }
    {
        printTest("Test 14: USER with all parameters (no reply, username set)");
        Client client(4);
        line = "USER ssin 0 * :Sara Sin";
        expected = "";
        expectHandleLine(client, line, expected);
        expectUsername(client, "ssin");
    }
    {
        printTest("Test 15: NICK completes the registration (welcome block)");
        // Situation:  client already sent PASS and USER,
        //             only nickname is missing
        // Action: NICK completes the registration
        // Expected: the whole welcome block (001, 002, 003, 004, 422).
        // The host is set because the 001 line shows nick!user@host.
        Client client(4);
        client.setHostname("127.0.0.1");
        client.setPasswordOk();
        client.setUsername("ssin");
        line = "NICK ssin";
        expected = WELCOME;
        expectHandleLine(client, line, expected);
    }
    {
        printTest("Test 16: USER completes the registration (welcome block)");
        // Situation: password and nickname are set, the username is missing.
        // Action: USER completes the registration.
        // Expected: the whole welcome block
        Client client(4);
        client.setHostname("127.0.0.1");
        client.setPasswordOk();
        client.setNickname("ssin");
        line = "USER ssin 0 * :Sara Sin";
        expected = WELCOME;
        expectHandleLine(client, line, expected);
    }
    {
        printTest("Test 17: PASS completes the registration (welcome block)");
        // Situation: nickname and username are set, the password is missing
        //            (some clients send PASS last).
        // Action: the right password completes the registration.
        // Expected: the whole welcome block.
        Client client(4);
        client.setHostname("127.0.0.1");
        client.setNickname("ssin");
        client.setUsername("ssin");
        line = "PASS poulettepass";
        expected = WELCOME;
        expectHandleLine(client, line, expected);
    }
    {
        printTest("Test 18: NICK without the password (no welcome)");
        // Situation: only the username is set, the password is missing.
        // Action:  NICK, but the client is still not registered.
        // Expected: no output. The welcome must wait for the password.
        // No host: the server sends nothing, so the host is not used
        Client client(4);
        client.setUsername("ssin");
        line = "NICK ssin";
        expected = "";
        expectHandleLine(client, line, expected);
    }
    {
        printTest("Test 19: NICK after the welcome (no second welcome)");
        // Situation: the client is registered and was already welcomed.
        //            setWelcomeOk is a shortcut for "the block was sent".
        // Action: the client changes its nickname.
        // Expected: no output. The welcome is sent only once
        // No host: the server sends nothing, so the host is not used
        Client client(4);
        client.setPasswordOk();
        client.setNickname("ssin");
        client.setUsername("ssin");
        client.setWelcomeOk();
        line = "NICK sara";
        expected = "";
        expectHandleLine(client, line, expected);
    }

    std::cout << "\n"
              << (g_total - g_failed) << "/" << g_total << " tests passed"
              << std::endl;

    if (g_failed) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
