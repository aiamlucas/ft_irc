#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "Message.hpp"
#include "Parser.hpp"
#include "colors.hpp"

static int g_total = 0;
static int g_failed = 0;

// {"#42berlin", "hey pessoal!"} -> "<#42berlin><hey pessoal!>"
static std::string paramsToString(const std::vector<std::string>& params) {
    std::string result;
    for (std::size_t i = 0; i < params.size(); i++)
        result += "<" + params[i] + ">";
    return result;
}

// format the message to match this pattern:
// "prefix=[...] command=[...] params=[<param_1><param_2>]"
static std::string format(const Message& message) {
    return "prefix=[" + message.prefix + "] command=[" + message.command +
           "] params=[" + paramsToString(message.params) + "]";
}

static void expectParse(const std::string& line, const std::string& expected) {
    std::string input = format(parseLine(line));
    g_total++;
    if (input == expected) {
        std::cout << "\n"
                  << CL_GREEN << "OK: " << RESET << "\"" << line << "\""
                  << "\n"
                  << input << std::endl;
        return;
    }
    g_failed++;
    std::cerr << CL_RED << "FAIL: " << RESET << "\"" << line << "\"\n"
              << " expected: " << expected << "\n"
              << " input:    " << input << std::endl;
}

int main() {
    std::string line;
    std::string expected;

    // Test 1
    line = "PING";
    expected = "prefix=[] command=[PING] params=[]";
    expectParse(line, expected);

    // Test 2
    line = "NICK ssin";
    expected = "prefix=[] command=[NICK] params=[<ssin>]";
    expectParse(line, expected);

    // Test 3
    line = "JOIN #42berlin";
    expected = "prefix=[] command=[JOIN] params=[<#42berlin>]";
    expectParse(line, expected);

    // Test 4
    line = ":ssin!abcd@192.168.1.105 PRIVMSG #42berlin :oieee pessoal!";
    expected =
        "prefix=[ssin!abcd@192.168.1.105] command=[PRIVMSG] "
        "params=[<#42berlin><oieee pessoal!>]";
    expectParse(line, expected);

    // Test 5
    // extra spaces (should be ignored)
    line = ":ssin!@bc   PRIVMSG           #sk8  :aee";
    expected = "prefix=[ssin!@bc] command=[PRIVMSG] params=[<#sk8><aee>]";
    expectParse(line, expected);

    // Test 6
    // a ':' inside the trailing param, should be a text
    line = "PRIVMSG #sk8 :hey: there!";
    expected = "prefix=[] command=[PRIVMSG] params=[<#sk8><hey: there!>]";
    expectParse(line, expected);

    // Test 8
    // more than 15 params: keep the first 15
    line = "MODE #42 +o 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17";
    expected =
        "prefix=[] command=[MODE] "
        "params=[<#42><+o><1><2><3><4><5><6><7><8><9><10><11><12><13>]";
    expectParse(line, expected);

    // Test 13
    // empty line: command stays empty
    line = "";
    expected = "prefix=[] command=[] params=[]";
    expectParse(line, expected);

    std::cout << "\n"
              << (g_total - g_failed) << "/" << g_total << " tests passed"
              << std::endl;

    if (g_failed) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
