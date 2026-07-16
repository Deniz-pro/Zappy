/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Incantation.hpp
*/

#ifndef INCANTATION_HPP_
    #define INCANTATION_HPP_
    #include <vector>

struct IncantationState {
    int x = 0;
    int y = 0;
    int level = 0;
    std::vector<int> playerIds;

    bool active = true;
    float flashTimer = 0.0f;
    bool flashSuccess = false;
};

struct IncantationMessage {
    std::string text;
};

#endif
