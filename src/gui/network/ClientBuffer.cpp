/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** ClientBuffer.cpp
*/

#include "gui/network/Client.hpp"
#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <cerrno>

void Client::disconnect()
{
    if (_fd != -1)
        close(_fd);
    _fd = -1;
    _buffer.clear();
    _outbox.clear();
}

bool Client::extractLine(std::string &line)
{
    size_t pos = _buffer.find('\n');

    if (pos == std::string::npos)
        return false;
    line = _buffer.substr(0, pos);
    _buffer.erase(0, pos + 1);
    if (!line.empty() && line.back() == '\r')
        line.pop_back();
    return true;
}

void Client::handleRecvError(ssize_t bytesReceived, bool blocking)
{
    if (bytesReceived == 0) {
        disconnect();
        if (blocking)
            throw std::runtime_error("connection closed");
        return;
    }
    if (!blocking && (errno == EAGAIN || errno == EWOULDBLOCK))
        return;
    disconnect();
    if (blocking)
        throw std::runtime_error("recv failed");
}

void Client::readIntoBuffer(bool blocking)
{
    char buf[4096];
    ssize_t bytesReceived;

    if (_fd == -1)
        return;
    bytesReceived = recv(_fd, buf, sizeof(buf), 0);
    if (bytesReceived > 0) {
        _buffer.append(buf, static_cast<size_t>(bytesReceived));
        return;
    }
    handleRecvError(bytesReceived, blocking);
}

std::string Client::readLineBlocking()
{
    std::string line;

    while (!extractLine(line))
        readIntoBuffer(true);
    return line;
}
