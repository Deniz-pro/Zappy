/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** ACommands.hpp
*/

#ifndef ACOMMANDS_HPP
    #define ACOMMANDS_HPP
    #include "GUIEvents.hpp"
    #include "ICommands.hpp"
    #include "Client.hpp"
    #include "state/GameState.hpp"
    #include <unordered_map>
    #include <functional>

namespace zappy {

    class Server;

    class ACommands : public ICommands {
        public:
            ACommands(int clientId, GameState &game_state, GUIEvents &events, Server *server);
            void dispatch(const Command &command) override;
        protected:
            using Handler = std::function<void(const std::string &arg)>;
            std::unordered_map<CommandType, Handler> _cmds;
            virtual void init() = 0;
            int _clientId;
            GameState &_gameState;
            GUIEvents &_events;
            Server *_server;
            Client *getClient() const;
    };
}

#endif
