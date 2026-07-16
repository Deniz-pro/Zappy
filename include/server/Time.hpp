/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** Time.hpp
*/

#ifndef TIME_HPP_
    #define TIME_HPP_

#include <vector>
#include <utility>
#include <string>
#include "CommandParser.hpp"


namespace zappy {

    inline std::vector<std::pair<CommandType, int>> Time_TABLE = {
        {CommandType::BROADCAST, 7},
        {CommandType::CONNECT_NBR, 0},
        {CommandType::EJECT, 7},
        {CommandType::FORK, 42},
        {CommandType::FORWARD, 7},
        {CommandType::INCANTATION, 300},
        {CommandType::INVENTORY, 1},
        {CommandType::LEFT, 7},
        {CommandType::LOOK, 7},
        {CommandType::RIGHT, 7},
        {CommandType::SET, 7},
        {CommandType::TAKE, 7},
    };


}

#endif