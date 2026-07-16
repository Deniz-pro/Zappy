/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Victory.cpp
*/

#include "server/Server.hpp"
#include <iostream>

static constexpr int MAX_LEVEL = 8;
static constexpr int WIN_COUNT = 6;

void zappy::Server::notifyWin(const std::string &team)
{
    for (auto &client : _clients) {
        client.toSend += "seg " + team + "\n";
        client.response = true;
    }
}

void zappy::Server::checkVictory()
{
    if (_gameOver)
        return;
    const auto &players = _gameState.getPlayers();
    std::unordered_map<std::string, int> levelcount;

    for (const auto &pair : players) {
        if (pair.second.level == MAX_LEVEL)
            levelcount[pair.second.team]++;
    }
    for (const auto &pair : levelcount) {
        if (pair.second >= WIN_COUNT) {
            notifyWin(pair.first);
            _gameOver = true;
            return;
        }
    }
}
