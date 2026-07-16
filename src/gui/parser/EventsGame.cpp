/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** EventsGame.cpp
*/

#include "gui/parser/Commands.hpp"

void Commands::pgt(std::istringstream &stream, GameState &state)
{
    char hash = 0;
    int id = 0;
    int resourceIndex = 0;

    stream >> hash >> id >> resourceIndex;
    auto it = state.getPlayers().find(id);
    if (it == state.getPlayers().end())
        return;
    state.decrementTileResource(it->second.x, it->second.y, resourceIndex);
    state.incrementPlayerResource(id, resourceIndex);
}

void Commands::pdr(std::istringstream &stream, GameState &state)
{
    char hash = 0;
    int id = 0;
    int resourceIndex = 0;

    stream >> hash >> id >> resourceIndex;
    auto it = state.getPlayers().find(id);
    if (it == state.getPlayers().end())
        return;
    state.incrementTileResource(it->second.x, it->second.y, resourceIndex);
    state.decrementPlayerResource(id, resourceIndex);
}

void Commands::seg(std::istringstream &stream, GameState &state)
{
    std::string team = "";

    stream >> team;
    state.setWinner(team);
}

void Commands::sgt(std::istringstream &stream, GameState &state)
{
    int freq = 0;

    stream >> freq;
    state.freq = freq;
}

void Commands::smg(std::istringstream &stream, GameState &state)
{
    std::string msg;

    std::getline(stream, msg);
    if (!msg.empty() && msg[0] == ' ')
        msg.erase(0, 1);
    state.setServerMessage(msg, 4.0f);
}

void Commands::pfk(std::istringstream &stream, GameState &state)
{
    char hash = 0;
    int id = 0;

    stream >> hash >> id;
    (void)state;
}

void Commands::sst(std::istringstream &stream, GameState &state)
{
    int freq = 0;

    stream >> freq;
    state.freq = freq;
}

void Commands::pex(std::istringstream &stream, GameState &state)
{
    char hash = 0;
    int id = 0;

    stream >> hash >> id;
    auto it = state.getPlayers().find(id);
    if (it == state.getPlayers().end())
        return;
    state.addExpulsion(it->second.x, it->second.y);
    state.setServerMessage("Player #" + std::to_string(id) +
    " expelled players", 3.0f);
}

void Commands::suc(std::istringstream &stream, GameState &state)
{
    (void)stream;
    state.setServerMessage("[GUI] Unknown command sent to server", 4.0f);
}

void Commands::sbp(std::istringstream &stream, GameState &state)
{
    (void)stream;
    state.setServerMessage("[GUI] Bad parameter sent to server", 4.0f);
}
