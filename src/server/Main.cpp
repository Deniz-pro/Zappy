/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Main.cpp
*/

#include "Help.hpp"
#include "Epitech.hpp"
#include "ParseArgs.hpp"
#include "Server.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    zappy::ParseArgs parser(argc, argv);
    zappy::Params config = parser.parseFlags();
    zappy::Server server(config);

    if (help(argc, argv)) {
        print_help();
        return EXIT_SUCCESS;
    }
    try {
        server.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
