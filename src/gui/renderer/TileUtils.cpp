/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** TileUtils.cpp
*/

#include "gui/renderer/TileUtils.hpp"

Color TileUtils::getDominantColor(const Tile &tile)
{
    int maxVal = 0;
    int maxIdx = -1;

    for (int i = 0; i < 7; i++) {
        if (tile.resources[i] > maxVal) {
            maxVal = tile.resources[i];
            maxIdx = i;
        }
    }
    if (maxIdx == -1)
        return DARKGRAY;
    return RESOURCE_COLORS[maxIdx];
}
