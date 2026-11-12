#include "Channel.hpp"
#include "colors.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

static int g_total = 0;
static int g_failed = 0;

static void expectBool(bool actual, bool expected, const std::string& testName) {
    g_total++;
    if (actual == expected) {
        std::cout << CL_GREEN << "OK: " << RESET << testName << std::endl;
    } else {
        g_failed++;
        std::cerr << CL_RED << "FAIL: " << RESET << testName 
                  << " (expected " << (expected ? "true" : "false") 
                  << ", got " << (actual ? "true" : "false") << ")" << std::endl;
    }
}

static void expectEqual(size_t actual, size_t expected, const std::string& testName) {
    g_total++;
    if (actual == expected) {
        std::cout << CL_GREEN << "OK: " << RESET << testName << std::endl;
    } else {
        g_failed++;
        std::cerr << CL_RED << "FAIL: " << RESET << testName 
                  << " (expected " << expected << ", got " << actual << ")" << std::endl;
    }
}

int main() {
    std::cout << "--- Testing Channel ---" << std::endl;
    Channel chan("#test");

    // Test 1: First member joins and becomes operator
    chan.join(4);
    expectBool(chan.isMember(4), true, "Client 4 is member");
    expectBool(chan.isOperator(4), true, "Client 4 is operator (first member)");

    // Test 2: Second member joins, is NOT operator
    chan.join(5);
    expectBool(chan.isMember(5), true, "Client 5 is member");
    expectBool(chan.isOperator(5), false, "Client 5 is not operator");

    // Test 3: Duplicate join (should not change size)
    chan.join(4);
    expectEqual(chan.getMembers().size(), 2, "Duplicate join does not increase size");

    // Test 4: Second member leaves
    chan.part(5);
    expectBool(chan.isMember(5), false, "Client 5 left channel");

    // Test 5: First member (operator) leaves, channel becomes empty
    chan.part(4);
    expectEqual(chan.getMembers().size(), 0, "Channel is empty after last member leaves");

    std::cout << "\n" << (g_total - g_failed) << "/" << g_total << " tests passed" << std::endl;

    if (g_failed) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}