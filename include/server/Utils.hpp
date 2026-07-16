/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** Utils.hpp
*/

#ifndef UTILS_HPP
    #define UTILS_HPP

    #include "AICommands.hpp"
    #include "ParseArgs.hpp"
    #include <string>
    #include <sys/socket.h>
    #include <unordered_map>

    namespace zappy {
        class Utils {
            public:
                Utils();
                ~Utils() = default;
                static std::string tileTostring(const Tile &tile,
                    const std::unordered_map<int, Player> &players,
                    int x, int y);
                static std::string buildTileContent(
                    const std::unordered_map<int, Player> &players, const Tile &tile,
                    int x, int y, int selfId);
                static int getCommandTime(CommandType type);
                static std::string verify_incantation(Client &client, GameState _gameState);
                static int getShortestDelta(int from, int to, int size);
                static int computeDirection(
                    const Player &emitter,
                    const Player &receiver,
                    int w, int h);
        };        
    }

#endif