/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Client.hpp
*/

#ifndef CLIENT_HPP
    #define CLIENT_HPP
    #include <string>
    #include <sys/types.h>

class Client {
    public:
        Client();
        ~Client();
        Client(const Client &) = delete;
        Client &operator=(const Client &) = delete;
        void connectTo(const std::string &host, int port);
        void handshake();
        void sendMessage(const std::string &msg);
        void flushSend();
        std::string receive();
        bool isConnected() const;
    private:
        void disconnect();
        bool extractLine(std::string &line);
        void handleRecvError(ssize_t bytesReceived, bool blocking);
        void readIntoBuffer(bool blocking);
        std::string readLineBlocking();
        int _fd;
        std::string _buffer;
        std::string _outbox;
};

#endif
