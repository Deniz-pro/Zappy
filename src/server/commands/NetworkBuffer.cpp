/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** NetworkBuffer.cpp
*/

#include <stdexcept>
#include "NetworkBuffer.hpp"

namespace zappy {
    constexpr std::size_t MAX_QUEUE_SIZE = 10;

    NetworkBuffer::NetworkBuffer() : _buffer(""),  _parser(), _command_queue()
    {}

    void NetworkBuffer::AppendData(const std::string &data)
    {
        _buffer += data;
        extract_commands();
    }

    bool NetworkBuffer::has_command() const
    {
        return !_command_queue.empty();
    }

    Command NetworkBuffer::pop_command()
    {
        Command cmd;

        if (_command_queue.empty())
            throw std::runtime_error("No command available");
        cmd = _command_queue.front();
        _command_queue.pop();
        return cmd;
    }

    void NetworkBuffer::extract_commands()
    {
        Command cmd;
        std::size_t pos = _buffer.find("\n");

        while (pos != std::string::npos) {
            std::string line = _buffer.substr(0, pos);
            _buffer = _buffer.substr(pos + 1);
            if (!line.empty() && line.back() == '\r')
                line.pop_back();
            if (_command_queue.size() < MAX_QUEUE_SIZE) {
                cmd = _parser.parseCommand(line);
                _command_queue.push(cmd);
            }
            pos = _buffer.find('\n');
        }
    }
}