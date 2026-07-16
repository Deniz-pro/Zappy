/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** CommandParser.cpp
*/

#include "CommandParser.hpp"
#include <stdexcept>

namespace zappy {
    CommandParser::CommandParser() : _CommandMap({
        {"Forward", CommandType::FORWARD}, {"Right", CommandType::RIGHT},
        {"Left", CommandType::LEFT}, {"Look", CommandType::LOOK},
        {"Inventory", CommandType::INVENTORY},
        {"Broadcast", CommandType::BROADCAST},
        {"Connect_nbr", CommandType::CONNECT_NBR}, {"Fork", CommandType::FORK},
        {"Eject", CommandType::EJECT}, {"Take", CommandType::TAKE},
        {"Set", CommandType::SET}, {"Incantation", CommandType::INCANTATION},
        {"msz", CommandType::MSZ}, {"bct", CommandType::BCT},
        {"mct", CommandType::MCT}, {"tna", CommandType::TNA},
        {"ppo", CommandType::PPO}, {"plv", CommandType::PLV},
        {"pin", CommandType::PIN}, {"sgt", CommandType::SGT},
        {"sst", CommandType::SST},
    })
    {}

    Command CommandParser::BuildCommand(const std::string &keyword,
        const std::string &arg) const
    {
        const auto it = _CommandMap.find(keyword);

        if (it == _CommandMap.end())
            return {CommandType::UNKNOWN, {}};
        return {it->second, arg};
    }

    Command CommandParser::parseCommand(const std::string &line) const
    {
        const auto spacePos = line.find(' ');
        std::string keyword = line.substr(0, spacePos);
        std::string arg = "";

        if (spacePos != std::string::npos)
            arg = line.substr(spacePos + 1);
        return BuildCommand(keyword, arg);
    }
}
