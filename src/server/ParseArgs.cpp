/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** ParseArgs.cpp
*/

#include "ParseArgs.hpp"

zappy::ParseArgs::ParseArgs(int argc, char **argv) : _argv(argv), _argc(argc)
{
}

void zappy::ParseArgs::getTeamsNames(Params &params)
{
    if (optarg)
        params.teams.push_back(optarg);
    while (optind < _argc && _argv[optind][0] != '-')
        params.teams.push_back(_argv[optind++]);
}

void zappy::ParseArgs::managePort(Params &params)
{
    int port = std::stoi(optarg);

    if (port >= 1 && port <= 65535)
        params.port = port;
}

void zappy::ParseArgs::manageFlags(Params &params)
{
    _flags['p'] = [&params, this]() { managePort(params); };
    _flags['x'] = [&params]() { params.width = std::stoi(optarg); };
    _flags['y'] = [&params]() { params.height = std::stoi(optarg); };
    _flags['c'] = [&params]() { params.clientsNb = std::stoi(optarg); };
    _flags['f'] = [&params]() { params.freq = std::stoi(optarg); };
    _flags['n'] = [&params, this]() { getTeamsNames(params); };
}

zappy::Params zappy::ParseArgs::parseFlags()
{
    Params params;
    int opt = 0;

    manageFlags(params);
    while ((opt = getopt(_argc, _argv, "p:x:y:n:c:f:")) != -1) {
        auto it = _flags.find(static_cast<char>(opt));
        if (it != _flags.end())
            it->second();
    }
    return params;
}
