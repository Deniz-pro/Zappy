/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** GameStatePlayer.cpp
*/

#include "state/GameState.hpp"
#include "gui/renderer/Config.hpp"

void GameState::setPlayer(const Player &player)
{
    _players[player.id] = player;
}

void GameState::registerEgg(int id, int playerId, int x, int y)
{
    zappy::Egg egg;

    egg.id = id;
    egg.x = x;
    egg.y = y;
    auto it = _players.find(playerId);
    if (it != _players.end())
        egg.team = it->second.team;
    _eggs[id] = egg;
}

void GameState::removePlayer(int id)
{
    _players.erase(id);
    _broadcasts.erase(id);
}

void GameState::setEggAnim(int id, zappy::EggAnim anim)
{
    auto it = _eggs.find(id);

    if (it == _eggs.end())
        return;
    it->second.anim = anim;
    if (anim == zappy::EggAnim::HATCHING)
        it->second.animTimer = HATCH_DURATION;
    else if (anim == zappy::EggAnim::DYING)
        it->second.animTimer = DIE_DURATION;
}
