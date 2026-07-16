/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Incantation.cpp
*/

#include "Elevation.hpp"
#include "AICommands.hpp"
#include "ParseArgs.hpp"
#include <string>
#include <sys/socket.h>
#include <iostream>

namespace zappy {

    std::vector<int> AICommands::getParticipants(const Player &player)
    {
        std::vector<int> participants;

        for (const auto &entry : _gameState.getPlayers()) {
            const auto &p = entry.second;
            if (p.x == player.x && p.y == player.y && p.level == player.level)
                participants.push_back(p.id);
        }
        return participants;
    }

    bool AICommands::checkIncantationConditions(const Player &player)
    {
        const zappy::ElevationReq &req = zappy::ELEVATION_TABLE[player.level - 1];

        int nbr_players = 0;
        for (const auto &entry : _gameState.getPlayers()) {
            const Player &p = entry.second;
            if (p.x == player.x && p.y == player.y && p.level == player.level)
                nbr_players++;
        }
        if (nbr_players < req.players)
            return false;
        const auto &tile = _gameState.getTiles()[player.y][player.x];
        if (tile.resources[1] < req.linemate) {return false;}
        if (tile.resources[2] < req.deraumere) {return false;}
        if (tile.resources[3] < req.sibur) {return false;}
        if (tile.resources[4] < req.mendiane) {return false;}
        if (tile.resources[5] < req.phiras) {return false;}
        if (tile.resources[6] < req.thystame) {return false;}
        return true;
    }

    void AICommands::sendIncantation()
    {
        Client *client = getClient();

        if (!client)
            return;
        Player &player = client->player;
        if (player.in_elevation == false) {
            if (!checkIncantationConditions(player)) {
                client->toSend   += "ko\n";
                client->player._last_frequency = 0;
                client->response  = true;
                return;
            }
            std::vector<int> participants = getParticipants(player);
            for (int id : participants) {
                Player p = _gameState.getPlayers().at(id);
                p.in_elevation = true;
                if (p.id != player.id) {
                    _gameState.pushIncantationMessage(
                    p.id, {"Elevation underway\n"});
                }
            }
            _events.notifyPic(player, participants);
            client->toSend += "Elevation underway\n";
            client->player.first_response = true;
            client->player.my_elevation = true;
            client->player._last_frequency = 300;
            client->player.in_elevation = true;
            client->player.my_frequency = std::chrono::steady_clock::now();
            client->response = true;
            return;
        }
        std::vector<int> participants = getParticipants(player);
        if (!checkIncantationConditions(player)) {
            for (int id : participants) {
                Player p = _gameState.getPlayers().at(id);
                if (p.in_elevation == false)
                    continue;
                p.in_elevation = false;
                if (p.id != player.id) {
                    _gameState.pushIncantationMessage(
                        p.id,
                        {"ko\n"});
                }
                _gameState.setPlayer(p);
            }
            _events.notifyPie(player, false);
            client->toSend += "ko\n";
            player.in_elevation = false;
            player.my_elevation = false;
            client->response = true;
            return;
        }
        const zappy::ElevationReq &req = zappy::ELEVATION_TABLE[player.level - 1];
        for (int id : participants) {
            Player p = _gameState.getPlayers().at(id);
            p.level++;
            p.in_elevation = false;
            if (p.id != player.id) {
                _gameState.pushIncantationMessage(
                    p.id, { "Current level: " 
                        + std::to_string(p.level) + "\n"});
            }
            _gameState.setPlayer(p);
        }
        auto &tile = _gameState.getTiles()[player.y][player.x];
        int res[7] = {
        tile.resources[0], tile.resources[1],
        tile.resources[2], tile.resources[3],
        tile.resources[4], tile.resources[5],
        tile.resources[6]
        };
        res[1] -= req.linemate;
        res[2] -= req.deraumere;
        res[3] -= req.sibur;
        res[4] -= req.mendiane;
        res[5] -= req.phiras;
        res[6] -= req.thystame;
        _gameState.setTileResources(player.x, player.y, res);
        player.level++;
        player.in_elevation = false;
        player.my_elevation = false;
        _events.notifyPie(player, true);
        client->toSend  += "Current level: " + std::to_string(player.level) + "\n";
        client->response = true;
    }

}
