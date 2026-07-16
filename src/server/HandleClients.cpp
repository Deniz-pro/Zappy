/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** HandleClients.cpp
*/

#include "Utils.hpp"
#include "Encapsulations.hpp"
#include "Server.hpp"
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <bits/stdc++.h>

void zappy::Server::manageClients()
{
    for (std::size_t i = _fds.size(); i-- > 1;) {
        if (_fds[i].revents & (POLLERR | POLLHUP | POLLNVAL)) {
            disconnectClient(i);
            continue;
        }
        Client *client = getClient(_fds[i].fd);
        if (client != nullptr && client->dead && client->toSend.empty()) {
            disconnectClient(i);
            continue;
        }
        handleClient(i);
    }
}

void zappy::Server::checkNewResponse()
{
    for (auto &client : _clients) {
        if (client.response) {
            activatePollout(client.fd);
            client.response = false;
        }
    }
}

void zappy::Server::handleNewConnection()
{
    sockaddr_in address = {};
    socklen_t len = sizeof(address);
    int clientFd = Network::safe_accept(_fd, (struct sockaddr *)&address, &len);
    Client client = {};

    client.fd = clientFd;
    client.toSend = WELCOME;
    _clients.push_back(std::move(client));
    _fdToClient[clientFd] = &_clients.back();
    _fds.push_back({clientFd, POLLIN | POLLOUT, 0});
}

void zappy::Server::handleClient(std::size_t index)
{
    Client *client = getClient(_fds[index].fd);

    if (client == nullptr)
        return;
    if (_fds[index].revents & (POLLERR | POLLHUP | POLLNVAL)) {
        disconnectClient(index);
        return;
    }
    if (_fds[index].revents & POLLIN || client->networkBuffer.has_command())
        handleRead(index, *client);
    if (_fds[index].revents & POLLOUT || client->player.in_elevation)
        handleWrite(index, *client);
}

bool zappy::Server::sendClient(Client &client)
{
    ssize_t n = 0;

    IncantationMessage incMsg;
    BroadcastMessage broadcast;

    if (_gameState.popIncantationMessage(
            client.player.id, incMsg)) {
        client.toSend += incMsg.text;
        client.player.first_response = true;
        if (incMsg.text.find("Elevation underway") == 0)
            client.player.in_elevation = true;
        if (incMsg.text.find("Current level") == 0 || incMsg.text.find("ko") == 0) {
            if (incMsg.text.find("Current level") == 0)
                client.player.level++;
            client.player.in_elevation    = false;
            client.player._last_frequency = 0;
        }
    } else if (!client.player.in_elevation
        && _gameState.popBroadcast(
            client.player.id, broadcast)) {
        client.toSend += broadcast.text;
        client.player.first_response = true;
    }
    if (client.toSend.empty() && !client.player.my_elevation)
        return true;

    auto now = std::chrono::steady_clock::now();
    double elapsed =
        std::chrono::duration<double>(
            now - client.player.my_frequency).count();

    if (client.player.first_response == true) {
        client.player.first_response = false;
    }
    if (elapsed >=
        (float)client.player._last_frequency / _gameState.freq
        || client.type != ClientType::AI) {
                client.player.my_frequency =
            std::chrono::steady_clock::now();

        if (client.player.in_elevation == true && client.player.my_elevation == true) {
            Command incantation = {
                zappy::CommandType::INCANTATION,
                ""
            };
            client.handler->dispatch(incantation);
            client.player.in_elevation = false;
            client.player.my_elevation = false;
        }

        client.player._last_frequency = 0;
    } else
        return true;

    n = Network::safe_send(
        client.fd,
        client.toSend.data(),
        client.toSend.size(),
        MSG_NOSIGNAL);

    if (n <= 0)
        return false;

    client.toSend.erase(
        0,
        static_cast<std::size_t>(n));

    return true;
}

void zappy::Server::disconnectClient(std::size_t index) noexcept
{
    int fd = _fds[index].fd;
    Client *client = getClient(fd);

    if (client && client->type == ClientType::AI) {
        _events.notifyPdi(client->player);
        _gameState.removePlayer(client->player.id);
    }
    close(fd);
    _fds.erase(_fds.begin() + index);
    _fdToClient.erase(fd);
    _clients.erase(std::remove_if(_clients.begin(), _clients.end(),
        [fd](const Client &c) { return c.fd == fd; }), _clients.end());
}

zappy::Client *zappy::Server::getClient(int fd)
{
    auto it = _fdToClient.find(fd);
    if (it == _fdToClient.end())
        return nullptr;
    return it->second;
}

void zappy::Server::spawnEggs()
{
    Egg egg;

    for (const auto &team : _params.teams) {
        for (int i = 0; i < _params.clientsNb; ++i) {
            egg.x = rand() % _params.width;
            egg.y = rand() % _params.height;
            egg.team = team;
            _gameState.addEgg(egg);
        }
        _gameState.teamSlots[team] = _params.clientsNb;
    }
}
