/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** Manage_commands.hpp
*/

#ifndef AICOMMANDS_HPP
    #define AICOMMANDS_HPP

    #include "ACommands.hpp"
    #include <string>

namespace zappy {
    class AICommands : public ACommands {
        void sendForward();
        void sendRight();
        void sendLeft();
        void sendLook();
        void sendInventory();
        void sendConnectNbr();
        void sendFork();
        void sendEject();
        void ejectPlayer(const Player &other, const Player &ejector);
        void pushEjected(Player &player, Orientation orientation);
        void sendTake(const std::string &cmd);
        void sendIncantation();
        bool checkIncantationConditions(const Player &player);
        std::vector<int> getParticipants(const Player &player);
        void sendSet(const std::string &cmd);
        int getShortestDelta(int from, int to, int size);
        int computeDirection(const Player &emitter, const Player &receiver, int w, int h);
        void sendBroadcast(const std::string &cmd);
        public:
            AICommands(int clientId, GameState &game_sate, GUIEvents &events, Server *srv);
        protected:
            void init() override;
    };
}

#endif
