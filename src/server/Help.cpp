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
    for (int i = 1; i < ac; ++i)
        if (std::strcmp(av[i], "--help") == 0)
            return true;
    return false;
}

void print_help()
{
    std::cout << "USAGE: ./zappy_server -p port -x width -y height"
    << " -n name1 name2 ... -c clientsNb -f freq\n"
    << "\t-p port\t\tport number\n"
    << "\t-x width\t\twidth of the world\n"
    << "\t-y height\theight of the world\n"
    << "\t-n name1 name2 ...\tname of the team\n"
    << "\t-c clientsNb\tnumber of authorized clients per team\n"
    << "\t-f freq\t\treciprocal of time unit for execution of actions\n";
}
