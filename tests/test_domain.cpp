#include <cstdlib>
#include <iostream>

#include "Channel.hpp"
#include "ServerState.hpp"
#include "colors.hpp"

static int g_total = 0;
static int g_failed = 0;

// Boolean natijalarni tekshirish
static void expectBool(bool actual, bool expected,
                       const std::string& testName) {
    g_total++;
    if (actual == expected) {
        std::cout << CL_GREEN << "OK: " << RESET << testName << std::endl;
    } else {
        g_failed++;
        std::cerr << CL_RED << "FAIL: " << RESET << testName << " (expected "
                  << (expected ? "true" : "false") << ", got "
                  << (actual ? "true" : "false") << ")" << std::endl;
    }
}

// Sonlarni (size_t) tekshirish
static void expectEqual(size_t actual, size_t expected,
                        const std::string& testName) {
    g_total++;
    if (actual == expected) {
        std::cout << CL_GREEN << "OK: " << RESET << testName << std::endl;
    } else {
        g_failed++;
        std::cerr << CL_RED << "FAIL: " << RESET << testName << " (expected "
                  << expected << ", got " << actual << ")" << std::endl;
    }
}

int main() {
    std::cout << "--- Testing ServerState (Domain) ---" << std::endl;

    ServerState state;

    // Test 1: Kanal mavjud emasligini tekshirish
    Channel* chan = state.getChannel("#test");
    expectBool(chan == NULL, true, "Channel #test does not exist initially");

    // Test 2: mijozni kanalga qoshish
    state.handleJoin(4, "#test");

    // Test 3: Kanal yaratilganini tekshirish
    chan = state.getChannel("#test");
    expectBool(chan != NULL, true, "Channel #test is created after join");

    // Test 4: Azolar sonini tekshirish
    expectEqual(chan->getMembers().size(), 1, "Channel has 1 member");

    // Test 5: Birinchi azo operator ekanligini tekshirish
    expectBool(chan->isOperator(4), true,
               "Client 4 is operator (first member)");

    // Test 6: Ikkinchi mijozni fd = 5 qoshish
    state.handleJoin(5, "#test");
    expectBool(chan->getMembers().size(), 2,
               "Channel has 2 members after second join");

    // Test 7: Ikkinchi azo operator emasligini tekshirish
    expectBool(chan->isOperator(5), false, "Client 5 is NOT operator");

    // Test 8: Dublikat Join (fd=4 yana qo'shilmoqchi)
    state.handleJoin(4, "#test");
    expectEqual(chan->getMembers().size(), 2,
                "Duplicate join does not increase member count");

    // Natijalar
    std::cout << "\n"
              << (g_total - g_failed) << "/" << g_total << " tests passed"
              << std::endl;

    if (g_failed) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
