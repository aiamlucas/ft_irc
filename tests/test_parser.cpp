#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "Message.hpp"
#include "Parser.hpp"

#define GREEN "\033[32m"
#define RED "\033[31m"
#define COLOR_OFF "\033[0m"

static int g_total = 0;
static int g_failed = 0;

// {"#42berlin", "hey pessoal!"} -> "<#42berlin><hey pessoal!>"
static std::string paramsToString(const std::vector<std::string>& params) {
    std::string result;
    for (std::size_t i = 0; i < params.size(); i++)
        result += "<" + params[i] + ">";
    return result;
}

static void expectParse(const std::string& line, const std::string& prefix,
                        const std::string& command, const std::string& params) {
    Message parsedLine = parseLine(line);
    g_total++;
    if (parsedLine.prefix == prefix && parsedLine.command == command &&
        paramsToString(parsedLine.params) == params) {
        std::cout << "\n"
                  << GREEN << "OK: " << COLOR_OFF << "\"" << line << "\""
                  << std::endl;
        return;
    }
    g_failed++;
    std::cerr << RED << "FAIL: " << COLOR_OFF << "\"" << line << "\"\n"
              << " expected: prefix=[" << prefix << "] command=[" << command
              << "] params=[" << params << "]" << "\n"
              << " got: prefix=[" << parsedLine.prefix << "] command=["
              << parsedLine.command << "] params=["
              << paramsToString(parsedLine.params) << "]" << std::endl;
}

int main() {
    // testing the line and the expected results

    // expectParse(line, prefix, command, params)

    // line: the raw text, that comes from the client
    //       [:prefix] COMMAND [param1, param2, ...] [:trailing]

    // Test 1
    // "PING"
    // prefix: ""
    // command: PING
    // params:  ""
    expectParse("PING", "", "PING", "");

    // Test 2
    // "NICK ssin"
    // prefix: ""
    // command: NICK
    // params: ssin
    expectParse("NICK ssin", "", "NICK", "<ssin>");

    // Test 3
    // "JOIN #42berlin"
    // prefix: ""
    // command: JOIN
    // params: #42berlin
    expectParse("JOIN #42berlin", "", "JOIN", "<#42berlin>");

    // Test 4
    // ":ssin!abcd@192.168.1.105 PRIVMSG #42berlin :oieee pessoal!"
    // prefix: ssin!abcd@192.168.1.105
    // command: PRIVMSG
    // params: #42berlin
    //         oieee pessoal!
    expectParse(":ssin!abcd@192.168.1.105 PRIVMSG #42berlin :oieee pessoal!",
                "ssin!abcd@192.168.1.105", "PRIVMSG",
                "<#42berlin><oieee pessoal!>");

    // Test 5
    // extra spaces (should be ignored)
    expectParse(":ssin!@bc   PRIVMSG           #sk8  :aee", "ssin!@bc",
                "PRIVMSG", "<#sk8><aee>");

    // Test 6
    // a ':' inside the trailing param, should be a text
    expectParse("PRIVMSG #sk8 :hey: there!", "", "PRIVMSG",
                "<#sk8><hey: there!>");

    // Test 7
    // line ends at "\r\n"
    expectParse("JOIN #archlinux\r\n", "", "JOIN", "<#archlinux>");

    std::cout << "\n"
              << (g_total - g_failed) << "/" << g_total << " tests passed"
              << std::endl;

    if (g_failed) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
