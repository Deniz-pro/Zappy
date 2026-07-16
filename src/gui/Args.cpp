/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Args.cpp
*/

#include "Args.hpp"
#include <stdexcept>

int Args::parsePort(const char *value)
{
    try {
        return std::stoi(value);
    } catch (const std::exception &) {
        throw std::invalid_argument("invalid port: " + std::string(value));
    }
}

Args Args::fill(int ac, char **av)
{
    Args args;

    for (int i = 1; i < ac; i++) {
        std::string arg = av[i];
        if (arg == "-p" && i + 1 < ac)
            args._port = parsePort(av[++i]);
        else if (arg == "-h" && i + 1 < ac)
            args._host = av[++i];
    }
    return args;
}

Args Args::parse(int ac, char **av)
{
    Args args = fill(ac, av);

    if (args._port == -1)
        throw std::invalid_argument("missing -p port");
    return args;
}
