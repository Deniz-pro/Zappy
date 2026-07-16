/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Elevation.hpp
*/

#ifndef ELEVATION_HPP_
    #define ELEVATION_HPP_

#include <vector>

namespace zappy {

    struct ElevationReq {
        int level;
        int players;
        int linemate;
        int deraumere;
        int sibur;
        int mendiane;
        int phiras;
        int thystame;
    };

    inline std::vector<ElevationReq> ELEVATION_TABLE = {
        {1, 1, 1, 0, 0, 0, 0, 0},
        {2, 2, 1, 1, 1, 0, 0, 0},
        {3, 2, 2, 0, 1, 0, 2, 0},
        {4, 4, 1, 1, 2, 0, 1, 0},
        {5, 4, 1, 2, 1, 3, 0, 0},
        {6, 6, 1, 2, 3, 0, 1, 0},
        {7, 6, 2, 2, 2, 2, 2, 1},
    };

}
#endif