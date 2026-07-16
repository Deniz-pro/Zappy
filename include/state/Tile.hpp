/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Tile.hpp
*/

#ifndef TILE_HPP_
    #define TILE_HPP_

inline constexpr const char *RESOURCE_NAMES[7] = {"f", "l", "d", "s", "m", "p", "t"};

struct Tile {
    int resources[7] = {0};
};

#endif
