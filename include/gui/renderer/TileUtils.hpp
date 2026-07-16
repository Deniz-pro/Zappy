/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** TileUtils.hpp
*/

#ifndef TILEUTILS_HPP_
    #define TILEUTILS_HPP_
    #include "state/Tile.hpp"
    #include <raylib.h>

inline constexpr Color RESOURCE_COLORS[7] = {
    GREEN, BLUE, ORANGE, YELLOW, PURPLE, RED, PINK
};

inline constexpr Color RESOURCE_SHAPE_COLORS[7] = {
    {0, 120, 40, 255}, {20, 60, 180, 255},
    {180, 80, 0, 255}, {160, 140, 0, 255},
    {120, 0, 160, 255}, {160, 0, 20, 255}, {180, 0, 100, 255},
};

class TileUtils {
    public:
        static Color getDominantColor(const Tile &tile);
};

#endif
