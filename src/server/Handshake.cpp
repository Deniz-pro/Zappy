/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** Handshake.cpp
*/

#include "Server.hpp"
#include <iostream>

void zappy::Server::parseHandshake(Client &client)
{
    std::size_t pos = client.handshakeBuf.find('\n');
    std::string line;

    while (pos != std::string::npos) {
        line = client.handshakeBuf.substr(0, pos);
        client.handshakeBuf.erase(0, pos + 1);
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (!line.empty())
            handleHandshake(client, line);
        pos = client.handshakeBuf.find('\n');
    }
}

void zappy::Server::handleGUIHandshake(Client &client)
{
    client.type = ClientType::GUI;
    client.handshakeDone = true;
    client.id = client.fd;
    client.handler = std::make_unique<GUICommands>(client.id, _gameState, _events, this);
    client.toSend += "ok\n";
    for (const auto &[id, egg] : _gameState.getEggs()) {
        client.toSend += "enw #" + std::to_string(egg.id) + " #0 " 
        + std::to_string(egg.x) + " " + std::to_string(egg.y) + "\n";
    }
    activatePollout(client.fd);
    std::cout << "GUI client connected.\n";
}

void zappy::Server::handleEgg(Player &p, const std::string &team)
{
    for (const auto &[id, egg] : _gameState.getEggs()) {
        if (egg.team == team) {
            p.x = egg.x;
            p.y = egg.y;
            _events.notifyEbo(egg);
            _gameState.removeEgg(egg.id);
            break;
        }
    }
}

void zappy::Server::handleAIHandshake(Client &client, const std::string &team)
{
    Player p;
    int playerId = 0;

    _gameState.teamSlots[team]--;
    client.type = ClientType::AI;
    client.handshakeDone = true;
    p.team = team;
    p.orientation = static_cast<Orientation>((rand() % 4) + 1);
    p.level = 1;
    handleEgg(p, team);
    p.inventory[0] = 10;
    playerId = _gameState.addPlayer(p);
    p.id = playerId;
    _gameState.setPlayer(p);
    client.player = p;
    client.id = playerId;
    client.handler = std::make_unique<AICommands>(client.id, _gameState, _events, this);
    client.toSend += std::to_string(_gameState.teamSlots[team]) + "\n";
    client.toSend += std::to_string(_params.width) + " "
        + std::to_string(_params.height) + "\n";
    activatePollout(client.fd);
    _events.notifyPnw(p);
}

void zappy::Server::handleHandshake(Client &client, const std::string &team)
{
    if (team == GRAPHIC)
        return handleGUIHandshake(client);
    auto it = std::find(_params.teams.begin(), _params.teams.end(), team);
    if (it == _params.teams.end()) {
        client.toSend += "ko\n";
        activatePollout(client.fd);
        return;
    }
    if (_gameState.teamSlots.find(team) != _gameState.teamSlots.end()
        && _gameState.teamSlots[team] > 0)
        return handleAIHandshake(client, team);
    client.toSend += "ko\n";
    activatePollout(client.fd);
}
