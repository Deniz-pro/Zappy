/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** Client.hpp
*/

#ifndef CLIENT_HPP
    #define CLIENT_HPP
    #include "NetworkBuffer.hpp"
    #include "ICommands.hpp"
    #include "state/Player.hpp"
    #include <memory>
    #include <string>

namespace zappy {
    enum class ClientType {
        AI,
        GUI,
        UNKNOWN
    };

    struct Client {
        int id = 0;
        int fd = -1;
        bool dead = false;
        std::string handshakeBuf;
        NetworkBuffer networkBuffer;
        std::string toSend;
        ClientType type = ClientType::UNKNOWN;
        bool handshakeDone = false;
        bool response = false;
        std::unique_ptr<ICommands> handler;
        Player player;
    };

}

#endif
