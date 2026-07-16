/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** ACommands.cpp
*/

#include "ACommands.hpp"
#include "ParseArgs.hpp"
#include "Server.hpp"
#include "Utils.hpp"

zappy::ACommands::ACommands(int clientId, GameState &state, GUIEvents &events, Server *srv)
    : _clientId(clientId), _gameState(state), _events(events), _server(srv)
{
}

zappy::Client *zappy::ACommands::getClient() const
{
    if (!_server)
        return nullptr;
    return _server->getClientId(_clientId);
}

void zappy::ACommands::dispatch(const Command &command)
{
    Client *client = getClient();

    if (!client)
        return;
    if (command.type == CommandType::UNKNOWN) {
        (client->type == ClientType::AI) ? client->toSend += "ko\n"
            : client->toSend += "suc\n";
        client->response = true;
        return;
    }
    auto it = _cmds.find(command.type);
    if (it == _cmds.end()) {
        (client->type == ClientType::AI) ? client->toSend += "ko\n"
            : client->toSend += "suc\n";
        client->response = true;
        return;
    }
    client->player._last_frequency = zappy::Utils::getCommandTime(it->first);
    it->second(command.arg);
}
