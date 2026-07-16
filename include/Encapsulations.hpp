/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** Encapsulations.hpp
*/

#ifndef ENCAPSULATIONS_HPP
    #define ENCAPSULATIONS_HPP
    #include <netinet/in.h>
    #include <poll.h>
    #include <stdexcept>
    #include <string>
    #include <sys/socket.h>

namespace zappy::Network {
    class NetworkFunctionFailed : public std::exception {
        std::string _message;
        public:
            NetworkFunctionFailed(std::string mess) : _message(mess) {};
            const char *what() const noexcept override {
                return _message.c_str();;
            }
    };

    inline int safe_socket(int domain, int type, int protocol)
    {
        int fd = socket(domain, type, protocol);

        if (fd < 0)
            throw NetworkFunctionFailed("Socket management failed.");
        return fd;
    }

    inline void safe_bind(int fd, const struct sockaddr *addr, socklen_t len)
    {
        int n = bind(fd, addr, len);

        if (n < 0)
            throw NetworkFunctionFailed("Bind management failed.");
    }

    inline void safe_listen(int fd, int n)
    {
        int m = listen(fd, n);

        if (m < 0)
            throw NetworkFunctionFailed("Listen management failed.");
    }

    inline int safe_accept(int fd, struct sockaddr *addr, socklen_t *len)
    {
        int client = accept(fd, addr, len);

        if (client < 0)
            throw NetworkFunctionFailed("Accept management failed.");
        return client;
    }

    inline void safe_poll(struct pollfd *fds, nfds_t nfds, int timeout)
    {
        int res = poll(fds, nfds, timeout);

        if (res < 0)
            throw NetworkFunctionFailed("Poll management failed.");
    }

    inline void safe_setsockopt(int fd, int level, int optname,
        const void *optval, socklen_t optlen)
    {
        int n = setsockopt(fd, level, optname, optval, optlen);

        if (n < 0)
            throw NetworkFunctionFailed("Setsockopt management failed.");
    }

    inline ssize_t safe_recv(int fd, void *buf, size_t n, int flags)
    {
        ssize_t res = recv(fd, buf, n, flags);

        if (res < 0)
            throw NetworkFunctionFailed("Recv management failed.");
        return res;
    }

    inline ssize_t safe_send(int fd, const void *buf, size_t n, int flags)
    {
        ssize_t res = send(fd, buf, n, flags);

        if (res == -1)
            throw NetworkFunctionFailed("Send management failed.");
        return res;
    }

}

#endif
