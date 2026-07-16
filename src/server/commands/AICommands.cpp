/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** AICommands.cpp
*/

#include "AICommands.hpp"
#include "ParseArgs.hpp"
#include "Utils.hpp"
#include <string>
#include <cmath>
#include <sys/socket.h>
#include <unordered_map>
#include <iostream>

zappy::AICommands::AICommands(int clientId, GameState &game_state, GUIEvents &events, Server *srv)
    : ACommands(clientId, game_state, events, srv)
{
    init();
}

void zappy::AICommands::init()
{
    _cmds[CommandType::FORWARD] = [this](const std::string &)
    { sendForward(); };
    _cmds[CommandType::RIGHT] = [this](const std::string &) { sendRight(); };
    _cmds[CommandType::LEFT] = [this](const std::string &) { sendLeft(); };
    _cmds[CommandType::LOOK] = [this](const std::string &) { sendLook(); };
    _cmds[CommandType::INVENTORY] = [this](const std::string &)
    { sendInventory(); };
    _cmds[CommandType::BROADCAST] = [this](const std::string &arg)
    { sendBroadcast(arg); };
    _cmds[CommandType::CONNECT_NBR] = [this](const std::string &)
    { sendConnectNbr(); };
    _cmds[CommandType::FORK] = [this](const std::string &) { sendFork(); };
    _cmds[CommandType::EJECT] = [this](const std::string &) { sendEject(); };
    _cmds[CommandType::TAKE] = [this](const std::string &arg)
    { sendTake(arg); };
    _cmds[CommandType::SET] = [this](const std::string &arg) { sendSet(arg); };
    _cmds[CommandType::INCANTATION] = [this](const std::string &)
    { sendIncantation(); };
}

void zappy::AICommands::sendForward()
{
    Client *client = getClient();

    if (!client)
        return;
    Player &player = client->player;
    if (player.orientation == Orientation::NORTH) {
        player.y  = (player.y - 1 + _gameState.getMapHeight()) % _gameState.getMapHeight();
    } else if (player.orientation == Orientation::EAST) {
        player.x  = (player.x + 1) % _gameState.getMapWidth();
    } else if (player.orientation == Orientation::SOUTH) {
        player.y  = (player.y + 1) % _gameState.getMapHeight();
    } else {
        player.x = (player.x - 1 + _gameState.getMapWidth()) % _gameState.getMapWidth();
    }
    _gameState.setPlayer(player);
    _events.notifyPpo(player);
    client->toSend += "ok\n";
    client->response = true;
}

void zappy::AICommands::sendRight()
{
    Client *client = getClient();
    int ori = 0;

    if (!client)
        return;
    ori = static_cast<int>(client->player.orientation);
    client->player.orientation = static_cast<Orientation>(ori % 4 + 1);
    _events.notifyPpo(client->player);
    client->toSend += "ok\n";
    client->response = true;
}

void zappy::AICommands::sendLeft()
{
    Client *client = getClient();
    int ori = 0;

    if (!client)
        return;
    ori = static_cast<int>(client->player.orientation);
    client->player.orientation = static_cast<Orientation>((ori + 2) % 4 + 1);
    _events.notifyPpo(client->player);
    client->toSend += "ok\n";
    client->response = true;
}

void zappy::AICommands::sendLook()
{
    Client *client = getClient();

    if (!client)
        return;
    const Player &player = client->player;
    int level = player.level;
    int px = player.x;
    int py = player.y;
    int ori = static_cast<int>(player.orientation);
    int w = _gameState.getMapWidth();
    int h = _gameState.getMapHeight();
    const std::vector<std::vector<Tile>> &tiles = _gameState.getTiles();
    const std::unordered_map<int, Player> &players = _gameState.getPlayers();
    std::string result = "[ ";

    for (int dist = 0; dist <= level; dist++) {
        for (int side = -dist; side <= dist; side++) {
            int tx = px;
            int ty = py;
            if (ori == 1) {
                ty = (py - dist + h) % h;
                tx = (px + side + w) % w;
            } else if (ori == 2) {
                tx = (px + dist) % w;
                ty = (py + side + h) % h;
            } else if (ori == 3) {
                ty = (py + dist) % h;
                tx = (px - side + w) % w;
            } else {
                tx = (px - dist + w) % w;
                ty = (py - side + h) % h;
            }
            if (result != "[ ")
                result += ", ";
            if (dist == 0 && side == 0)
                result += Utils::buildTileContent(players, tiles[ty][tx], tx, ty, _clientId);
            else
                result += Utils::tileTostring(tiles[ty][tx], players, tx, ty);
        }
    }
    result += " ]\n";
    client->toSend += result;
    client->response = true;
}

void zappy::AICommands::sendTake(const std::string &cmd)
{
    Client *client = getClient();

    if (!client)
        return;
    std::string resource = cmd;
    if (!resource.empty() && resource.front() == '<')
    resource = resource.substr(1, resource.size() - 2);
    int x = client->player.x;
    int y = client->player.y;
    const std::vector<std::vector<Tile>> &tiles = _gameState.getTiles();
    const std::string names[7] = {
        "food", "linemate", "deraumere",
        "sibur", "mendiane", "phiras", "thystame"
    };
    int res[7] = {
        tiles[y][x].resources[0], tiles[y][x].resources[1],
        tiles[y][x].resources[2], tiles[y][x].resources[3],
        tiles[y][x].resources[4], tiles[y][x].resources[5],
        tiles[y][x].resources[6]
    };
    for (int r = 0; r < 7; r++) {
        if (resource == names[r] && res[r] > 0) {
            res[r]--;
            _gameState.setTileResources(x, y, res);
            client->player.inventory[r]++;
            client->toSend += "ok\n";
            client->response = true;
            _events.notifyPgt(client->player, r);
            _events.notifyPin(client->player);
            return;
        }
    }
    client->toSend += "ko\n";
    client->response = true;
}

void zappy::AICommands::sendFork()
{
    Client *client = getClient();
    Egg egg;
    int eggId = 0;
    
    if (!client)
        return;
    egg.x = client->player.x;
    egg.y = client->player.y;
    egg.team = client->player.team;
    eggId = _gameState.addEgg(egg);
    egg.id = eggId;
    _gameState.teamSlots[client->player.team]++;
    client->toSend += "ok\n";
    client->response = true;
    _events.notifyPfk(client->player);
    _events.notifyEnw(client->player, egg);
}

void zappy::AICommands::sendInventory()
{
    Client *client = getClient();
    const std::string resources[7] = {
        "food", "linemate", "deraumere",
        "sibur", "mendiane", "phiras", "thystame"};
    std::string result = "[ ";
        
    if (!client)
        return;
    for (int i = 0; i < 7; i++) {
        result += resources[i] + " ";
        result += std::to_string(std::max(0, client->player.inventory[i]));
        if (i < 6)
            result += ", ";
    }
    result += " ]\n";
    client->toSend += result;
    client->response = true;
    _events.notifyPin(client->player);
}

void zappy::AICommands::sendConnectNbr()
{
    Client *client = getClient();

    if (!client)
        return;
    int slots = _gameState.teamSlots[client->player.team];
    client->toSend += std::to_string(slots) + "\n";
    client->response = true;
}

void zappy::AICommands::sendSet(const std::string &cmd)
{
    Client *client = getClient();

    if (!client)
        return;
    std::string resource = cmd;
    if (!resource.empty() && resource.front() == '<')
        resource = resource.substr(1, resource.size() - 2);
    const std::string resources[7] = {
        "food", "linemate", "deraumere",
        "sibur", "mendiane", "phiras", "thystame"};
    Player &player = client->player;
    int index = -1;
    const auto &tiles = _gameState.getTiles();
    int res[7] = {};

    for (int i = 0; i < 7; ++i)
        if (resources[i] == resource)
            index = i;
    if (index < 0 || player.inventory[index] <= 0) {
        client->toSend += "ko\n";
        client->response = true;
        return;
    }
    for (int i = 0; i < 7; ++i)
        res[i] = tiles[player.y][player.x].resources[i];
    player.inventory[index]--;
    res[index]++;
    _gameState.setTileResources(player.x, player.y, res);
    _gameState.setPlayer(player);
    client->toSend += "ok\n";
    client->response = true;
    _events.notifyPdr(client->player, index);
    _events.notifyPin(client->player);
}

void zappy::AICommands::sendEject()
{
    Client *client = getClient();
    std::vector<int> toRemove;
    
    if (!client)
        return;
    if (client->player.in_elevation) {
        client->toSend += "ko\n";
        client->response = true;
        return;
    }
    for (const auto &entry : _gameState.getPlayers())
        ejectPlayer(entry.second, client->player);
    for (const auto &entry : _gameState.getEggs()) {
        const auto &egg = entry.second;
        if (egg.x == client->player.x && egg.y == client->player.y) {
            _events.notifyEdi(egg);
            toRemove.push_back(egg.id);
        }
    }
    for (int id : toRemove)
        _gameState.removeEgg(id);
    client->toSend += "ok\n";
    client->response = true;
}

void zappy::AICommands::ejectPlayer(const Player &other, const Player &ejector)
{
    if (other.id == ejector.id)
        return;
    if (other.x != ejector.x || other.y != ejector.y)
        return;
    Player pushed = other;
    pushEjected(pushed, ejector.orientation);
    _gameState.setPlayer(pushed);
    _events.notifyPex(pushed);
}

void zappy::AICommands::pushEjected(Player &player, Orientation orientation)
{
    switch (orientation) {
        case Orientation::NORTH:
            player.y = (player.y - 1 + _gameState.getMapHeight())
                % _gameState.getMapHeight();
            break;
        case Orientation::EAST:
            player.x = (player.x + 1) % _gameState.getMapWidth();
            break;
        case Orientation::SOUTH:
            player.y = (player.y + 1) % _gameState.getMapHeight();
            break;
        default:
            player.x = (player.x - 1 + _gameState.getMapWidth())
                % _gameState.getMapWidth();
    }
}

void zappy::AICommands::sendBroadcast(const std::string &cmd)
{
    Client *client = getClient();

    if (!client)
        return;
    const Player &emitter = client->player;
    int w = _gameState.getMapWidth();
    int h = _gameState.getMapHeight();
    for (auto &entry : _gameState.getPlayers()) {
        const Player &receiver = entry.second;
        if (receiver.id == emitter.id)
            continue;
        int k = Utils::computeDirection(emitter, receiver, w, h);
        BroadcastMessage msg;
        msg.text = "message " + std::to_string(k) + ", " + cmd + "\n";
        _gameState.pushBroadcast(receiver.id, msg);
    }
    _events.notifyPbc(emitter, cmd);
    client->toSend += "ok\n";
    client->response = true;
}
