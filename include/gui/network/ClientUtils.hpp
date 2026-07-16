/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** ClientUtils.hpp
*/

#ifndef CLIENTUTILS_HPP
    #define CLIENTUTILS_HPP
    #include <string>

class ClientUtils {
    public:
        static int openTcpSocket(const std::string &host, int port);
        static void sendPartial(int fd, std::string &buffer);
    private:
        static size_t sendAvailable(int fd, const std::string &buffer);
};

#endif
