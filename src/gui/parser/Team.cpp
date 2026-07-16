/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Team.cpp
*/

#include "gui/parser/Commands.hpp"

void Commands::tna(std::istringstream &stream, GameState &state)
{
    std::string name;

    stream >> name;
    state.addTeam(name);
}
