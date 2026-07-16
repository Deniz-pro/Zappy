/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Client.cpp
*/

#include "gui/network/Client.hpp"
#include "gui/network/ClientUtils.hpp"
#include <stdexcept>
#include <fcntl.h>

Client::Client() : _fd(-1)
{}

Client::~Client()
{
    disconnect();
}

void Client::connectTo(const std::string &host, int port)
{
    disconnect();
    _fd = ClientUtils::openTcpSocket(host, port);
}

void Client::handshake()
{
    if (readLineBlocking() != "WELCOME")
        throw std::runtime_error("expected WELCOME");
    sendMessage("GRAPHIC\n");
    sendMessage("msz\n");
    sendMessage("mct\n");
    sendMessage("tna\n");
    sendMessage("sgt\n");
    if (fcntl(_fd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("failed to set non blocking mode");
}

void Client::sendMessage(const std::string &msg)
{
    if (_fd == -1)
        return;
    _outbox += msg;
    flushSend();
}

void Client::flushSend()
{
    if (_fd == -1 || _outbox.empty())
        return;
    try {
        ClientUtils::sendPartial(_fd, _outbox);
    } catch (const std::exception &) {
        disconnect();
    }
}

std::string Client::receive()
{
    std::string line;

    if (_fd == -1)
        return "";
    if (!extractLine(line)) {
        readIntoBuffer(false);
        if (!isConnected() || !extractLine(line))
            return "";
    }
    return line;
}

bool Client::isConnected() const
{
    return _fd != -1;
}
