/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** HandleCommands.cpp
*/

#include <iostream>
#include "Encapsulations.hpp"
#include "Server.hpp"

void zappy::Server::handleRead(std::size_t index, Client &client)
{
    if (_fds[index].revents & POLLIN) {
        if (!readCommands(client)) {
            disconnectClient(index);
            return;
        }
    }
    if (client.player._last_frequency == 0 && !client.player.in_elevation) {
        parseCommands(client);
    }
}

void zappy::Server::handleWrite(std::size_t index, Client &client)
{
    if (!sendClient(client)) {
        disconnectClient(index);
        return;
    }
    if (client.toSend.empty() && client.player._last_frequency == 0)
        _fds[index].events = POLLIN;
}

bool zappy::Server::readCommands(Client &client)
{
    char buf[MAX_BUFFER] = {};
    ssize_t n = Network::safe_recv(client.fd, buf, sizeof(buf), 0);

    if (n <= 0)
        return false;

    if (!client.handshakeDone) {
        client.handshakeBuf.append(buf, n);
    } else {
        std::string received(buf, n);
        size_t start = 0;
        size_t pos;
        while ((pos = received.find('\n', start)) != std::string::npos) {
            std::string command = received.substr(start, pos - start);
            if (!command.empty() && command.back() == '\r')
                command.pop_back();
            command += "\n";
            client.networkBuffer.AppendData(command);
            start = pos + 1;
        }
        if (start < received.size())
        {
            client.networkBuffer.AppendData(
                received.substr(start)
            );
        }
    }
    return true;
}

void zappy::Server::parseCommands(Client &client)
{
    Command cmd;

    if (!client.handshakeDone) {
        parseHandshake(client);
        return;
    }
    if (client.player.inventory[0] < 0) {
        return;
    }
    if (client.networkBuffer.has_command()) {
        cmd = client.networkBuffer.pop_command();
        client.handler->dispatch(cmd);
    }
}
