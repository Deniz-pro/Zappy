/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** GameStateEffects.cpp
*/

#include "state/GameState.hpp"
#include "gui/renderer/Config.hpp"

void GameState::addIncantation(int x, int y, int level,
    const std::vector<int> &playerIds)
{
    IncantationState state;

    state.level = level;
    state.x = x;
    state.y = y;
    state.playerIds = playerIds;
    _incantations[tileKey(x, y)] = state;
    for (int id : playerIds) {
        auto it = _players.find(id);
        if (it != _players.end())
            it->second.in_elevation = true;
    }
}

void GameState::endIncantation(int x, int y, bool success)
{
    auto it = _incantations.find(tileKey(x, y));

    if (it == _incantations.end())
        return;
    for (int id : it->second.playerIds) {
        auto pit = _players.find(id);
        if (pit == _players.end())
            continue;
        pit->second.in_elevation = false;
        if (success)
            pit->second.level = it->second.level + 1;
    }
    it->second.active = false;
    it->second.flashTimer = FLASH_DURATION;
    it->second.flashSuccess = success;
}

void GameState::setWinner(const std::string &team)
{
    _winner = team;
}

void GameState::setServerMessage(const std::string &msg, float duration)
{
    _serverMessage = msg;
    _serverMessageTimer = duration;
}

void GameState::pushBroadcast(int playerId, const std::string &text)
{
    BroadcastMessage msg;

    msg.text = text;
    msg.timer = BROADCAST_DURATION;
    auto &deque = _broadcasts[playerId];
    while (static_cast<int>(deque.size()) >= BROADCAST_MAX_MESSAGES)
        deque.pop_front();
    deque.push_back(msg);
}

void GameState::addExpulsion(int x, int y)
{
    _expulsions[tileKey(x, y)] = FLASH_DURATION;
}


void GameState::tickExpulsions(float dt)
{
    for (auto it = _expulsions.begin(); it != _expulsions.end();) {
        it->second -= dt;
        if (it->second <= 0.0f)
            it = _expulsions.erase(it);
        else
            ++it;
    }
}

void GameState::tickIncantations(float dt)
{
    for (auto it = _incantations.begin(); it != _incantations.end();) {
        if (!it->second.active)
            it->second.flashTimer -= dt;
        if (!it->second.active && it->second.flashTimer <= 0.0f)
            it = _incantations.erase(it);
        else
            it++;
    }
}

void GameState::tickEggs(float dt)
{
    for (auto it = _eggs.begin(); it != _eggs.end();) {
        auto &egg = it->second;
        if (egg.anim == zappy::EggAnim::IDLE) {
            ++it;
            continue;
        }
        egg.animTimer -= dt;
        if (egg.animTimer <= 0.0f)
            it = _eggs.erase(it);
        else
            ++it;
    }
}

void GameState::tickBroadcasts(float dt)
{
    for (auto it = _broadcasts.begin(); it != _broadcasts.end();) {
        auto &deque = it->second;
        if (deque.empty()) {
            it = _broadcasts.erase(it);
            continue;
        }
        for (auto &msg : deque)
            msg.timer -= dt;
        while (!deque.empty() && deque.front().timer <= 0.0f)
            deque.pop_front();
        it++;
    }
}

void GameState::tickEffects(float dt)
{
    tickIncantations(dt);
    tickExpulsions(dt);
    tickEggs(dt);
    tickBroadcasts(dt);
    if (_serverMessageTimer > 0.0f) {
        _serverMessageTimer -= dt;
        if (_serverMessageTimer <= 0.0f)
            _serverMessage.clear();
    }
}
