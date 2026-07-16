/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Hunger.cpp
*/

#include "server/Server.hpp"

static const double HUNGER_INTERVAL = 126.0;

void zappy::Server::checkHunger()
{
    auto now = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(now - _lastHungerCheck).count();

    if (elapsed < HUNGER_INTERVAL / _gameState.freq)
        return;
    _lastHungerCheck = now;
    for (auto &client : _clients) {
        if (client.type != ClientType::AI)
            continue;
        if (client.player.inventory[0] < 0)
            continue;
        client.player.inventory[0]--;
        if (client.player.inventory[0] <= 0) {
            client.player.inventory[0] = -1;
            client.dead = true;
            client.toSend += "dead\n";
            client.response = true;
        }
    }
}
