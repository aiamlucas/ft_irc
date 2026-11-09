#include "../../includes/Channel.hpp"
#include <iostream>
#include <cassert>

int main() {
    std::cout << "--- Testing Channel ---" << std::endl;

    // 1. Kanal yaratish
    Channel chan("#test");

    // 2. Join testi
    chan.join(4); // Pedro (fd 4) qo'shildi
    assert(chan.isMember(4) == true);
    assert(chan.isOperator(4) == true); // Birinchi odam avtomatik operator
    std::cout << "Join test passed!" << std::endl;

    // 3. Ikkinchi odam qo'shilishi
    chan.join(5); // Ana (fd 5) qo'shildi
    assert(chan.isMember(5) == true);
    assert(chan.isOperator(5) == false); // Ikkinchi odam operator emas
    std::cout << "Second join test passed!" << std::endl;

    // 4. Dublikat join (Pedro yana kirmoqchi)
    chan.join(4); 
    assert(chan.getMembers().size() == 2); // A'zolar soni o'zgarmadi
    std::cout << "Duplicate join test passed!" << std::endl;

    // 5. Part testi (Ana chiqib ketdi)
    chan.part(5);
    assert(chan.isMember(5) == false);
    std::cout << "Part test passed!" << std::endl;

    // 6. Pedroning chiqishi (kanal bo'sh qolishi)
    chan.part(4);
    assert(chan.getMembers().empty() == true);
    std::cout << "Empty channel test passed!" << std::endl;

    std::cout << "ALL TESTS PASSED SUCCESSFULLY!" << std::endl;
    return 0;
}