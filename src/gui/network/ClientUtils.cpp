/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** ClientUtils.cpp
*/

#include "gui/network/ClientUtils.hpp"
#include "Epitech.hpp"
#include <stdexcept>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstddef>

static addrinfo *resolveHost(const std::string &host, int port)
{
    addrinfo hints{};
    addrinfo *res = nullptr;

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host.c_str(), std::to_string(port).c_str(),
    &hints, &res) != 0)
        throw std::runtime_error("cannot resolve host: " + host);
    return res;
}

static int connectToAddrInfo(addrinfo *res)
{
    int fd;

    for (addrinfo *p = res; p; p = p->ai_next) {
        fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (fd == -1)
            continue;
        if (connect(fd, p->ai_addr, p->ai_addrlen) == 0)
            return fd;
        close(fd);
    }
    return FAILURE;
}

int ClientUtils::openTcpSocket(const std::string &host, int port)
{
    addrinfo *res = resolveHost(host, port);
    int fd;

    fd = connectToAddrInfo(res);
    freeaddrinfo(res);
    if (fd == -1)
        throw std::runtime_error("connection failed to " +
            host + ":" + std::to_string(port));
    return fd;
}

static bool handleSendError()
{
    if (errno == EAGAIN || errno == EWOULDBLOCK)
        return true;
    if (errno == EINTR)
        return false;
    throw std::runtime_error("send failed");
}

size_t ClientUtils::sendAvailable(int fd, const std::string &buffer)
{
    size_t sent = 0;
    ssize_t bytesSent = 0;

    while (sent < buffer.size()) {
        bytesSent = send(fd, buffer.data() + sent,
            buffer.size() - sent, MSG_NOSIGNAL);
        if (bytesSent > 0) {
            sent += static_cast<size_t>(bytesSent);
            continue;
        }
        if (handleSendError())
            break;
    }
    return sent;
}

void ClientUtils::sendPartial(int fd, std::string &buffer)
{
    size_t sent = sendAvailable(fd, buffer);

    if (sent > 0)
        buffer.erase(0, sent);
}
