/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** NetworkBuffer.hpp
*/

#ifndef NETWORKBUFFER_HPP
    #define NETWORKBUFFER_HPP
    #include "CommandParser.hpp"
    #include <string>
    #include <queue>

namespace zappy {
    class NetworkBuffer {
        public:
            NetworkBuffer();
            void AppendData(const std::string &data);
            bool has_command() const;
            Command pop_command();
        private:
            std::string _buffer;
            CommandParser _parser;
            std::queue<Command> _command_queue;
            void extract_commands();
    };
}

#endif
