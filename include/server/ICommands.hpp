/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** ICommands.hpp
*/

#ifndef ICOMMANDS_HPP
    #define ICOMMANDS_HPP
    #include "CommandParser.hpp"
    #include <string>

namespace zappy {
    class ICommands {
        public:
            virtual ~ICommands() = default;
            virtual void dispatch(const Command &command) = 0;
    };
}

#endif
