/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** Server.hpp
*/

#ifndef SERVER_HPP
    #define SERVER_HPP
    constexpr int MAX_BUFFER = 4096;
    constexpr const char *GRAPHIC = "GRAPHIC";
    constexpr const char *WELCOME = "WELCOME\n";
    #include "CommandParser.hpp"
    #include "ICommands.hpp"
    #include "AICommands.hpp"
    #include "GUICommands.hpp"
    #include "Client.hpp"
    #include "ParseArgs.hpp"
    #include "GUIEvents.hpp"
    #include "state/GameState.hpp"
    #include <memory>
    #include <deque>
    #include <vector>
    #include <poll.h>
    #include <string>
    #include <chrono>
    #include <list>
    #include <unordered_map>

namespace zappy {
    class Server {
        void checkVictory();
        void checkHunger();
        void notifyWin(const std::string &team);
        void manageClients();
        void handleNewConnection();
        void checkNewResponse();
        void handleRead(std::size_t index, Client &client);
        void handleWrite(std::size_t index, Client &client);
        void handleClient(std::size_t index);
        void parseHandshake(Client &client);
        void parseCommands(Client &client);
        void handleEgg(Player &p, const std::string &team);
        void handleGUIHandshake(Client &client);
        void handleAIHandshake(Client &client, const std::string &team);
        void handleHandshake(Client &client, const std::string &team);
        bool readCommands(Client &client);
        bool sendClient(Client &client);
        void activatePollout(int fd) noexcept;
        void disconnectClient(std::size_t index) noexcept;
        void spawnResources();
        Client *getClient(int fd);
        int createSocket(int port);
        void spawnEggs();
        std::list<Client> _clients;
        std::unordered_map<int, Client *> _fdToClient;
        std::vector<pollfd> _fds;
        Params _params;
        int _fd;
        GameState _gameState;
        std::chrono::steady_clock::time_point _last_resource_spawn;
        std::chrono::steady_clock::time_point _lastHungerCheck;
        bool _gameOver = false;
        GUIEvents _events;
        public:
            Server(const Params &params);
            ~Server();
            void run();
            Client *getClientId(int id);
    };
}

#endif
