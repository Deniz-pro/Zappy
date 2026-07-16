/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Player.hpp
*/

#ifndef PLAYER_HPP
    #define PLAYER_HPP
    #include <string>
    #include <chrono>

enum class Orientation {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4
};

struct Player {
    int id = 0;
    int x = 0;
    int y = 0;
    bool in_elevation    = false;
    bool my_elevation    = false;
    bool first_response  = false;
    Orientation orientation = Orientation::NORTH;
    float  _last_frequency = 0;
    std::chrono::steady_clock::time_point my_frequency = std::chrono::steady_clock::now();
    int  level = 1;
    std::string team;
    int  inventory[7] = {0};
};

#endif
