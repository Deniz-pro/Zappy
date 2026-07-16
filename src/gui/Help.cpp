/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Help.cpp
*/

#include "Help.hpp"
#include <cstring>
#include <iostream>

bool help(int ac, char **av)
{
    for (int i = 1; i < ac; i++)
        if (std::strcmp(av[i], "--help") == 0)
            return true;
    return false;
}

void print_help()
{
    std::cout << "USAGE: ./zappy_gui -p port -h machine\n"
    << "\t-p port\t\tport number\n"
    << "\t-h machine\thostname of the server\n";
}
