/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Parser.cpp
*/

#include "gui/parser/Parser.hpp"
#include "gui/parser/Commands.hpp"
#include <sstream>
#include <unordered_map>

static const std::unordered_map<std::string,
    void(*)(std::istringstream &, GameState &)> HANDLERS = {
    {"msz", Commands::msz},
    {"bct", Commands::bct},
    {"tna", Commands::tna},
    {"pnw", Commands::pnw},
    {"ppo", Commands::ppo},
    {"plv", Commands::plv},
    {"pin", Commands::pin},
    {"pdi", Commands::pdi},
    {"pic", Commands::pic},
    {"pie", Commands::pie},
    {"pbc", Commands::pbc},
    {"enw", Commands::enw},
    {"ebo", Commands::ebo},
    {"edi", Commands::edi},
    {"pgt", Commands::pgt},
    {"pdr", Commands::pdr},
    {"seg", Commands::seg},
    {"sgt", Commands::sgt},
    {"smg", Commands::smg},
    {"pfk", Commands::pfk},
    {"pex", Commands::pex},
    {"sst", Commands::sst},
    {"suc", Commands::suc},
    {"sbp", Commands::sbp},
};


void Parser::parse(const std::string &line, GameState &state)
{
    std::istringstream stream(line);
    std::string cmd;

    if (line.empty())
        return;
    stream >> cmd;
    auto it = HANDLERS.find(cmd);
    if (it != HANDLERS.end())
        it->second(stream, state);
}
