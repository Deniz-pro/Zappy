/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Map.cpp
*/

#include "gui/parser/Commands.hpp"

void Commands::msz(std::istringstream &stream, GameState &state)
{
    int width = 0;
    int height = 0;

    stream >> width >> height;
    if (width <= 0 || height <= 0)
        return;
    state.setMapSize(width, height);
}

void Commands::bct(std::istringstream &stream, GameState &state)
{
    int x = 0;
    int y = 0;
    int resources[7] = {0};

    stream >> x >> y;
    if (x < 0 || x >= state.getMapWidth() || y < 0
    || y >= state.getMapHeight())
        return;
    for (int i = 0; i < 7; i++)
        stream >> resources[i];
    state.setTileResources(x, y, resources);
}
