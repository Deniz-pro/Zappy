/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** GUIEvents.cpp
*/

#include "GUIEvents.hpp"
#include <sstream>

zappy::GUIEvents::GUIEvents(std::list<Client> &clients) : _clients(clients)
{
}

void zappy::GUIEvents::sendEvent(const std::string &message)
{
    for (auto &client : _clients)
        if (client.type == ClientType::GUI) {
            client.toSend += message;
            client.response = true;
        }
}

void zappy::GUIEvents::notifyPpo(const Player &player)
{
    std::ostringstream os;

    os << "ppo #" << player.id << " " << player.x << " " << player.y
       << " " << static_cast<int>(player.orientation) << "\n";
    sendEvent(os.str());
}

void zappy::GUIEvents::notifyMct(const GameState &state)
{
    std::ostringstream os;

    for (int y = 0; y < state.getMapHeight(); y++) {
        for (int x = 0; x < state.getMapWidth(); x++) {
            const auto &tile = state.getTiles()[y][x];
            os << "bct " << x << " " << y;
            for (int i = 0; i < 7; i++)
                os << " " << tile.resources[i];
            os << "\n";
        }
    }
    sendEvent(os.str());
}

void zappy::GUIEvents::notifyPnw(const Player &player)
{
    std::ostringstream os;

    os << "pnw #" << player.id << " " << player.x << " " << player.y << " "
        << static_cast<int>(player.orientation) << " " << player.level << " "
        << player.team << "\n";
    sendEvent(os.str());
}

void zappy::GUIEvents::notifyPex(const Player &player)
{
    std::ostringstream os;

    os << "pex #" << player.id << "\n";
    sendEvent(os.str());
}

void zappy::GUIEvents::notifyPbc(const Player &player, const std::string message)
{
    std::ostringstream os;

    os << "pbc #" << player.id << " " << message << "\n";
    sendEvent(os.str());
}

void zappy::GUIEvents::notifyPic(const Player &player, const std::vector<int> &players)
{
    std::ostringstream os;

    os << "pic " << player.x << " " << player.y << " " << player.level;
    for (int id : players)
        os << " #" << id;
    os << "\n";
    sendEvent(os.str());
}

void zappy::GUIEvents::notifyPie(const Player &player, bool result)
{
    std::ostringstream os;

    os << "pie " << player.x << " " << player.y << " "
        << (result ? 1 : 0) << "\n";
    sendEvent(os.str());
}

void zappy::GUIEvents::notifyPlv(const Player &player)
{
    std::ostringstream os;

    os << "plv #" << player.id << " " << player.level << "\n";
    sendEvent(os.str());
}

void zappy::GUIEvents::notifyPfk(const Player &player)
{
    std::ostringstream os;

    os << "pfk #" << player.id << "\n";
    sendEvent(os.str());
}

void zappy::GUIEvents::notifyPdr(const Player &player, int resource)
{
    std::ostringstream os;

    os << "pdr #" << player.id << " " << resource << "\n";
    sendEvent(os.str());
}

void zappy::GUIEvents::notifyPgt(const Player &player, int resource)
{
    std::ostringstream os;

    os << "pgt #" << player.id << " " << resource << "\n";
    sendEvent(os.str());
}

void zappy::GUIEvents::notifyPdi(const Player &player)
{
    std::ostringstream os;

    os << "pdi #" << player.id << "\n";
    sendEvent(os.str());
}

void zappy::GUIEvents::notifyEnw(const Player &player, const Egg &egg)
{
    std::ostringstream os;

    os << "enw #" << egg.id << " #" << player.id << " " << egg.x
        << " " << egg.y << "\n";
    sendEvent(os.str());
}

void zappy::GUIEvents::notifyEbo(const Egg &egg)
{
    std::ostringstream os;

    os << "ebo #" << egg.id << "\n";
    sendEvent(os.str());
}

void zappy::GUIEvents::notifyEdi(const Egg &egg)
{
    std::ostringstream os;

    os << "edi #" << egg.id << "\n";
    sendEvent(os.str());
}

void zappy::GUIEvents::sendSmg(const std::string &message)
{
    std::ostringstream os;

    os << "smg " << message << "\n";
    sendEvent(os.str());
}

void zappy::GUIEvents::notifyPin(const Player &player)
{
    std::ostringstream os;

    os << "pin #" << player.id << " " << player.x << " " << player.y;
    for (int i = 0; i < 7; ++i)
        os << " " << std::max(0, player.inventory[i]);
    os << "\n";
    sendEvent(os.str());
}
