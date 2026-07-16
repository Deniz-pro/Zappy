/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** Utils.cpp
*/

#include "Utils.hpp"
#include "CommandParser.hpp"
#include "Time.hpp"
#include "Elevation.hpp"
#include <algorithm>
#include <cmath>

static constexpr const char *FULL_NAMES[7] = {
    "food", "linemate", "deraumere",
    "sibur", "mendiane", "phiras", "thystame"
};

std::string zappy::Utils::tileTostring(const Tile &tile,
    const std::unordered_map<int, Player> &players, int x, int y)
{
    std::string result = "";

    for (const auto &pair : players) {
        if (pair.second.x == x && pair.second.y == y) {
            if (!result.empty())
                result += " ";
            result += "player";
        }
    }
    for (int r = 0; r < 7; r++) {
        for (int i = 0; i < tile.resources[r]; i++) {
            if (!result.empty())
                result += " ";
            result += FULL_NAMES[r];
        }
    }
    return result;
}

std::string zappy::Utils::buildTileContent(
    const std::unordered_map<int, Player> &players,
    const Tile &tile, int x, int y, int selfId)
{
    std::string result = "";
    auto it = players.find(selfId);

    if (it != players.end() && it->second.x == x && it->second.y == y)
        result = "player";
    for (const auto &pair : players) {
        if (pair.first == selfId)
            continue;
        if (pair.second.x == x && pair.second.y == y) {
            if (!result.empty())
                result += " ";
            result += "player";
        }
    }
    for (int r = 0; r < 7; r++) {
        for (int i = 0; i < tile.resources[r]; i++) {
            if (!result.empty())
                result += " ";
            result += FULL_NAMES[r];
        }
    }
    return result;
}

int zappy::Utils::getCommandTime(CommandType type) {
        auto it = std::find_if(
            Time_TABLE.begin(),
            Time_TABLE.end(),
            [type](const std::pair<CommandType, int>& entry) {
                return entry.first == type;
            }
        );
        if (it == Time_TABLE.end())
            return -1;
        return it->second;
}

std::string zappy::Utils::verify_incantation(Client &_client, GameState _gameState)
{
        Player &player = _client.player;
        int nbr_players_need = zappy::ELEVATION_TABLE[player.level].level;
        int nbr_players  = 0;

        for (const auto &entry : _gameState.getPlayers()) {
            if (entry.second.level == player.level)
                nbr_players++;
        }
        if (nbr_players < nbr_players_need)
            return "ko\n";
        player.level++;
        std::string the_result = "Current level: " + std::to_string(player.level) + '\n';
        return the_result;
}

int zappy::Utils::getShortestDelta(int from, int to, int size)
{
    int direct = (to - from + size) % size;
    int reverse = (from - to + size) % size;

    return (direct <= reverse) ? direct : -reverse;
}

int zappy::Utils::computeDirection(
    const Player &emitter,
    const Player &receiver,
    int w, int h)
{
    int dx = getShortestDelta(emitter.x, receiver.x, w);
    int dy = getShortestDelta(emitter.y, receiver.y, h);

    if (dx == 0 && dy == 0)
        return 0;

    double angle = std::atan2(-dy, dx);
    double oriented = angle
        - (static_cast<int>(receiver.orientation) - 1) * (M_PI / 2.0);

    oriented = std::fmod(oriented + 2 * M_PI, 2 * M_PI);
    if (oriented < M_PI / 8 || oriented >= 15 * M_PI / 8)
        return 1;
    if (oriented < 3 * M_PI / 8)
        return 2;
    if (oriented < 5 * M_PI / 8)
        return 3;
    if (oriented < 7 * M_PI / 8)
        return 4;
    if (oriented < 9 * M_PI / 8)
        return 5;
    if (oriented < 11 * M_PI / 8)
        return 6;
    if (oriented < 13 * M_PI / 8)
        return 7;
    return 8;
}
