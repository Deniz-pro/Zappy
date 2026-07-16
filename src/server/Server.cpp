/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** Server.cpp
*/

#include "Encapsulations.hpp"
#include "Server.hpp"
#include "AICommands.hpp"
#include "GUICommands.hpp"
#include <iostream>
#include <netinet/in.h>

zappy::Server::Server(const Params &params) : _params(params), _fd(-1), _events(_clients)
{
    srand(time(nullptr));
    _gameState.freq = params.freq;
    _gameState.setMapSize(params.width, params.height);
    for (const auto &team : params.teams) {
        _gameState.addTeam(team);
        _gameState.teamSlots[team] = params.clientsNb;
    }
    spawnResources();
    spawnEggs();
    _last_resource_spawn = std::chrono::steady_clock::now();
    _lastHungerCheck = std::chrono::steady_clock::now();
}

zappy::Server::~Server()
{
    if (_fd > 0)
        close(_fd);
    for (auto &client : _clients)
        if (client.fd > 0)
            close(client.fd);
}

void zappy::Server::run()
{
    std::cout << "Listening on port " << _params.port << "\n";
    _fd = createSocket(_params.port);
    _fds.push_back({_fd, POLLIN, 0});
    while (true) {
        Network::safe_poll(_fds.data(), static_cast<nfds_t>(_fds.size()), 100);
        if (_fds[0].revents & POLLIN)
            handleNewConnection();
        manageClients();
        checkNewResponse();
        checkVictory();
        checkHunger();
        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(now - _last_resource_spawn).count();
        if (elapsed >= 20.0 / _gameState.freq) {
            spawnResources();
            _last_resource_spawn = now;
        }
    }
}

void zappy::Server::activatePollout(int fd) noexcept
{
    for (auto &pfd : _fds) {
        if (pfd.fd == fd) {
            pfd.events |= POLLOUT;
            return;
        }
    }
}

int zappy::Server::createSocket(int port)
{
    int fd = Network::safe_socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr = {};
    int opt = 1;

    Network::safe_setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(static_cast<uint16_t>(port));
    Network::safe_bind(fd, (struct sockaddr *)&addr, sizeof(addr));
    Network::safe_listen(fd, SOMAXCONN);
    return fd;
}

zappy::Client *zappy::Server::getClientId(int id)
{
    for (auto &client : _clients) {
        if (client.id == id)
            return &client;
    }
    return nullptr;
}