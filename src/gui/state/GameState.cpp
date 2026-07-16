/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** GameState.cpp
*/

#include "state/GameState.hpp"

int GameState::tileKey(int x, int y) const
{
    return y * _mapWidth + x;
}

void GameState::setMapSize(int width, int height)
{
    _mapWidth = width;
    _mapHeight = height;
    resizeMap();
    _mapChanged = true;
}

void GameState::resetMapChanged()
{
    _mapChanged = false;
}

void GameState::addTeam(const std::string &team)
{
    for (const auto &t : _teams)
        if (t == team)
            return;
    _teams.push_back(team);
}

void GameState::setTileResources(int x, int y, const int resources[7])
{
    if (x < 0 || x >= _mapWidth || y < 0 || y >= _mapHeight)
        return;
    for (int i = 0; i < 7; i++)
        _tiles[y][x].resources[i] = resources[i];
}

void GameState::decrementTileResource(int x, int y, int index)
{
    if (index < 0 || index >= 7)
        return;
    if (x < 0 || x >= _mapWidth || y < 0 || y >= _mapHeight)
        return;
    if (_tiles[y][x].resources[index] > 0)
        _tiles[y][x].resources[index]--;
}

void GameState::incrementTileResource(int x, int y, int index)
{
    if (index < 0 || index >= 7)
        return;
    if (x < 0 || x >= _mapWidth || y < 0 || y >= _mapHeight)
        return;
    _tiles[y][x].resources[index]++;
}

void GameState::incrementPlayerResource(int id, int index)
{
    if (index < 0 || index >= 7)
        return;
    auto it = _players.find(id);
    if (it == _players.end())
        return;
    it->second.inventory[index]++;
}

void GameState::decrementPlayerResource(int id, int index)
{
    if (index < 0 || index >= 7)
        return;
    auto it = _players.find(id);
    if (it == _players.end())
        return;
    if (it->second.inventory[index] > 0)
        it->second.inventory[index]--;
}

void GameState::resizeMap()
{
    _tiles.assign(_mapHeight, std::vector<Tile>(_mapWidth));
}

bool GameState::popBroadcast(int playerId,
    BroadcastMessage &msg)
{
    auto it = _broadcasts.find(playerId);

    if (it == _broadcasts.end() || it->second.empty())
        return false;
    msg = it->second.front();
    it->second.pop_front();
    if (it->second.empty())
        _broadcasts.erase(it);
    return true;
}

void GameState::pushIncantationMessage(
    int playerId,
    const IncantationMessage &msg)
{
    _incantations_msg[playerId].push(msg);
}

bool GameState::popIncantationMessage(
    int playerId,
    IncantationMessage &msg)
{
    auto it = _incantations_msg.find(playerId);

    if (it == _incantations_msg.end()
        || it->second.empty())
        return false;
    msg = it->second.front();
    it->second.pop();
    if (it->second.empty())
        _incantations_msg.erase(it);
    return true;
}