/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Parser.hpp
*/

#ifndef PARSER_HPP
    #define PARSER_HPP
    #include "state/GameState.hpp"
    #include <string>

class Parser {
    public:
        void parse(const std::string &line, GameState &state);
    private:
};

#endif
