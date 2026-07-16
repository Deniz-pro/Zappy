/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** GUICommands.cpp
*/

#include "GUICommands.hpp"
#include "ParseArgs.hpp"
#include "Server.hpp"
#include <sstream>

zappy::GUICommands::GUICommands(int clientId, GameState &game_state, GUIEvents &events, Server *srv)
    : ACommands(clientId, game_state, events, srv)
{
    init();
    sendInitialState();
}

void zappy::GUICommands::init()
{
    _cmds[CommandType::MSZ] = [this](const std::string &) { sendMsz(); };
    _cmds[CommandType::BCT] = [this](const std::string &num) { sendBct(num); };
    _cmds[CommandType::MCT] = [this](const std::string &) { sendMct(); };
    _cmds[CommandType::TNA] = [this](const std::string &) { sendTna(); };
    _cmds[CommandType::PPO] = [this](const std::string &num) { sendPpo(num); };
    _cmds[CommandType::PLV] = [this](const std::string &num) { sendPlv(num); };
    _cmds[CommandType::PIN] = [this](const std::string &num) { sendPin(num); };
    _cmds[CommandType::SGT] = [this](const std::string &) { sendSgt(); };
    _cmds[CommandType::SST] = [this](const std::string &num) { sendSst(num); };
}

void zappy::GUICommands::sendInitialState()
{
    sendMsz();
    sendMct();
    sendTna();
    sendPnw();
}

std::string zappy::GUICommands::removeHash(const std::string &s)
{
    if (!s.empty() && s[0] == '#')
        return s.substr(1);
    return s;
}

bool zappy::GUICommands::isValidInt(const std::string &s)
{
    if (s.empty())
        return false;
    for (char c : s)
        if (!std::isdigit(c) && c != '-')
            return false;
    return true;
}

bool zappy::GUICommands::parseBctArgs(const std::string &args, int &a, int &b)
{
    std::size_t space = args.find(' ');
    std::string first;
    std::string second;

    if (space == std::string::npos)
        return false;
    first = args.substr(0, space);
    second = args.substr(space + 1);
    if (!isValidInt(first) || !isValidInt(second))
        return false;
    a = std::stoi(first);
    b = std::stoi(second);
    return true;
}

void zappy::GUICommands::sendMsz()
{
    Client *client = getClient();
    std::ostringstream os;

    if (!client)
        return;

    os << "msz " << _gameState.getMapWidth() << " " << _gameState.getMapHeight() << "\n";
    client->toSend += os.str();
    client->response = true;
}

std::string zappy::GUICommands::buildBct(int x, int y)
{
    std::ostringstream os;
    const auto &tile = _gameState.getTiles()[y][x];

    os << "bct " << x << " " << y;
    for (int i = 0; i < 7; i++)
        os << " " << tile.resources[i];
    os << "\n";
    return os.str();
}

void zappy::GUICommands::sendBct(const std::string &pos)
{
    Client *client = getClient();
    int x = 0;
    int y = 0;

    if (!client)
        return;
    if (!parseBctArgs(pos, x, y))
        return sendSbp();
    if (x < 0 || x >= _gameState.getMapWidth() ||
        y < 0 || y >= _gameState.getMapHeight())
            return sendSbp();
    client->toSend += buildBct(x, y);
    client->response = true;
}

void zappy::GUICommands::sendMct()
{
    Client *client = getClient();

    if (!client)
        return;
    for (int y = 0; y < _gameState.getMapHeight(); y++)
        for (int x = 0; x < _gameState.getMapWidth(); x++)
            client->toSend += buildBct(x, y);
    client->response = true;
}

void zappy::GUICommands::sendTna()
{
    Client *client = getClient();

    if (!client)
        return;
    for (const auto &team : _gameState.getTeams())
        client->toSend += "tna " + team + "\n";
    client->response = true;
}

void zappy::GUICommands::sendPpo(const std::string &num)
{
    std::string number = removeHash(num);
    Client *client = getClient();
    std::ostringstream os;
    int id = 0;

    if (!client)
        return;
    if (!isValidInt(number))
        return sendSbp();
    id = std::stoi(number);
    auto it = _gameState.getPlayers().find(id);
    if (it == _gameState.getPlayers().end())
        return sendSbp();
    const auto &p = it->second;
    os << "ppo #" << p.id << " " << p.x << " " << p.y << " "
    << static_cast<int>(p.orientation) << "\n";
    client->toSend += os.str();
    client->response = true;
}

void zappy::GUICommands::sendPlv(const std::string &num)
{
    std::string number = removeHash(num);
    Client *client = getClient();
    std::ostringstream os;
    int id = 0;

    if (!client)
        return;
    if (!isValidInt(number))
        return sendSbp();
    id = std::stoi(number);
    auto it = _gameState.getPlayers().find(id);
    if (it == _gameState.getPlayers().end())
        return sendSbp();
    os << "plv #" << it->second.id << " " << it->second.level << "\n";
    client->toSend += os.str();
    client->response = true;
}

void zappy::GUICommands::sendPin(const std::string &num)
{
    std::string number = removeHash(num);
    Client *client = getClient();
    std::ostringstream os;
    int id = 0;

    if (!client)
        return;
    if (!isValidInt(number))
        return sendSbp();
    id = std::stoi(number);
    auto it = _gameState.getPlayers().find(id);
    if (it == _gameState.getPlayers().end())
        return sendSbp();
    const auto &p = it->second;
    os << "pin #" << p.id << " " << p.x << " " << p.y;
    for (int i = 0; i < 7; i++)
        os << " " << p.inventory[i];
    os << "\n";
    client->toSend += os.str();
    client->response = true;
}

void zappy::GUICommands::sendSgt()
{
    Client *client = getClient();
    std::ostringstream os;

    if (!client)
        return;
    os << "sgt " << _gameState.freq << "\n";
    client->toSend += os.str();
    client->response = true;
}

void zappy::GUICommands::sendSst(const std::string &freq)
{
    Client *client = getClient();
    std::ostringstream os;
    int new_freq = 0;

    if (!client)
        return;
    if (!isValidInt(freq))
        return sendSbp();
    new_freq = std::stoi(freq);
    if (new_freq < 1)
        return sendSbp();
    _gameState.freq = new_freq;
    os << "sst " << new_freq << "\n";
    client->toSend += os.str();
    client->response = true;
}

void zappy::GUICommands::sendSbp()
{
    Client *client = getClient();

    if (!client)
        return;
    client->toSend += SBP;
    client->response = true;
}

void zappy::GUICommands::sendPnw()
{
    Client *client = getClient();
    std::ostringstream os;

    if (!client)
        return;
    for (const auto &[id, p] : _gameState.getPlayers()) {
        os << "pnw #" << p.id << " " << p.x << " " << p.y
        << " " << static_cast<int>(p.orientation)
        << " " << p.level << " " << p.team << "\n";
        client->toSend += os.str();
    }
    client->response = true;
}
