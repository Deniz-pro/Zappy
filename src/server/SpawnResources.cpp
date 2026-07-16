/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** SpawnResources.cpp
*/

#include "server/Server.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>

static constexpr double DENSITIES[7] = {
    0.5, 0.3, 0.15, 0.1, 0.1, 0.08, 0.05
};

static int countResource(const std::vector<std::vector<Tile>> &tiles, int r)
{
    int total = 0;

    for (const auto &col : tiles) {
        for (const auto &tile : col) {
            total += tile.resources[r];
        }
    }
    return total;
}

static void add_Resource(GameState &state, int r)
{
    int x = std::rand() % state.getMapWidth();
    int y = std::rand() % state.getMapHeight();
    const std::vector<std::vector<Tile>> &tiles = state.getTiles();
    int res[7] = {
        tiles[y][x].resources[0], tiles[y][x].resources[1],
        tiles[y][x].resources[2], tiles[y][x].resources[3],
        tiles[y][x].resources[4], tiles[y][x].resources[5],
        tiles[y][x].resources[6]
    };
    res[r]++;
    state.setTileResources(x, y, res);
}

void zappy::Server::spawnResources()
{
    int total = _gameState.getMapWidth() * _gameState.getMapHeight();

    for (int r = 0; r < 7; r++) {
        int target = std::max(1, (int)(total * DENSITIES[r]));
        int current = countResource(_gameState.getTiles(), r);
        int to_add = target - current;
        for (int i = 0; i < to_add; i++) {
            add_Resource(_gameState, r);
        }
    }
    _events.notifyMct(_gameState);
}
