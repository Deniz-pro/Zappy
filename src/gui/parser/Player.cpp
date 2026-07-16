/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Player.cpp
*/

#include "gui/parser/Commands.hpp"

void Commands::pnw(std::istringstream &stream, GameState &state)
{
    Player p;
    char hash = 0;
    int ori = 0;

    stream >> hash >> p.id >> p.x >> p.y >> ori >> p.level >> p.team;
    p.orientation = static_cast<Orientation>(ori);
    state.setPlayer(p);
}

void Commands::ppo(std::istringstream &stream, GameState &state)
{
    char hash = 0;
    int id = 0;
    int x = 0;
    int y = 0;
    int ori = 0;

    stream >> hash >> id >> x >> y >> ori;
    auto it = state.getPlayers().find(id);
    if (it == state.getPlayers().end())
        return;
    Player p = it->second;
    p.x = x;
    p.y = y;
    p.orientation = static_cast<Orientation>(ori);
    state.setPlayer(p);
}

void Commands::plv(std::istringstream &stream, GameState &state)
{
    char hash = 0;
    int id = 0;
    int level = 0;
    Player p;

    stream >> hash >> id >> level;
    auto it = state.getPlayers().find(id);
    if (it == state.getPlayers().end())
        return;
    p = it->second;
    p.level = level;
    state.setPlayer(p);
}

void Commands::pin(std::istringstream &stream, GameState &state)
{
    char hash = 0;
    int id = 0;
    int x = 0;
    int y = 0;
    Player p;

    stream >> hash >> id >> x >> y;
    (void)x;
    (void)y;
    auto it = state.getPlayers().find(id);
    if (it == state.getPlayers().end())
        return;
    p = it->second;
    for (int i = 0; i < 7; i++)
        stream >> p.inventory[i];
    state.setPlayer(p);
}

void Commands::pdi(std::istringstream &stream, GameState &state)
{
    char hash = 0;
    int id = 0;

    stream >> hash >> id;
    state.removePlayer(id);
}
