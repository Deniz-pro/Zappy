/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Events.cpp
*/

#include "gui/parser/Commands.hpp"

void Commands::pic(std::istringstream &stream, GameState &state)
{
    int x = 0;
    int y = 0;
    int level = 0;
    std::vector<int> playerIds;
    char hash = 0;
    int id = 0;

    stream >> x >> y >> level;
    while (stream >> hash >> id)
        playerIds.push_back(id);
    state.addIncantation(x, y, level, playerIds);
}

void Commands::pie(std::istringstream &stream, GameState &state)
{
    int x = 0;
    int y = 0;
    int result = 0;

    stream >> x >> y >> result;
    state.endIncantation(x, y, result == 1);
}

void Commands::pbc(std::istringstream &stream, GameState &state)
{
    char hash = 0;
    int id = 0;
    std::string text;

    stream >> hash >> id;
    std::getline(stream, text);
    if (!text.empty() && text[0] == ' ')
        text.erase(0, 1);
    state.pushBroadcast(id, text);
}

void Commands::enw(std::istringstream &stream, GameState &state)
{
    char hash = 0;
    int eggId = 0;
    int playerId = 0;
    int x = 0;
    int y = 0;

    stream >> hash >> eggId >> hash >> playerId >> x >> y;
    state.registerEgg(eggId, playerId, x, y);
}

void Commands::ebo(std::istringstream &stream, GameState &state)
{
    char hash = 0;
    int id = 0;

    stream >> hash >> id;
    state.setEggAnim(id, zappy::EggAnim::HATCHING);
}

void Commands::edi(std::istringstream &stream, GameState &state)
{
    char hash = 0;
    int id = 0;

    stream >> hash >> id;
    state.setEggAnim(id, zappy::EggAnim::DYING);
}
