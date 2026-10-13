/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbueno-m <lbueno-m@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/13 09:25:10 by lbueno-m          #+#    #+#             */
/*   Updated: 2026/10/13 09:48:04 by lbueno-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdlib>
#include <iostream>
#include <string>

#include "Server.hpp"
#include "Utils.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string password = argv[2];
    int port;
    // Port numbers are 16-bit in TCP (0-65535) and 0 is already reserved for
    // auto-select (when the OS picks any free port).
    if (!safeAtoi(argv[1], port) || port < 1 || port > 65535) {
        std::cerr
            << "Error: invalid port (must be a number between 1 and 65535)"
            << std::endl;
        return EXIT_FAILURE;
    }
    if (password.empty()) {
        std::cerr << "Error: password cannot be empty" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
