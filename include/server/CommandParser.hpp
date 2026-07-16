/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** CommandParser.hpp
*/

#ifndef COMMANDPARSER_HPP
#define COMMANDPARSER_HPP

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

namespace zappy {
    enum class CommandType {
        FORWARD,
        RIGHT,
        LEFT,
        LOOK,
        INVENTORY,
        BROADCAST,
        CONNECT_NBR,
        FORK,
        EJECT,
        TAKE,
        SET,
        INCANTATION,
        MSZ,
        BCT,
        MCT,
        TNA,
        PPO,
        PLV,
        PIN,
        SGT,
        SST,
        UNKNOWN
    };

    struct Command {
        CommandType type;
        std::string arg;
    };

    class CommandParser {
        public:
            CommandParser();
            Command parseCommand(const std::string &line) const;
        private:
            std::unordered_map<std::string, CommandType> _CommandMap;
            Command BuildCommand(const std::string &keyword,
                const std::string &arg) const;
    };
}

#endif