/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** GUIEvents.hpp
*/

#ifndef GUIEVENTS_HPP
    #define GUIEVENTS_HPP
    #include "state/GameState.hpp"
    #include "Client.hpp"
    #include "state/Egg.hpp"
    #include <string>
    #include <vector>
    #include <list>

namespace zappy {
    class GUIEvents {
        void sendEvent(const std::string &message);
        std::list<Client> &_clients;
        public:
            GUIEvents(std::list<Client> &clients);
            void notifyPpo(const Player &player);
            void notifyMct(const GameState &state);
            void notifyPnw(const Player &player);
            void notifyPex(const Player &player);
            void notifyPbc(const Player &player, const std::string message);
            void notifyPic(const Player &player, const std::vector<int> &players);
            void notifyPie(const Player &player, bool result);
            void notifyPin(const Player &player);
            void notifyPlv(const Player &player);
            void notifyPfk(const Player &player);
            void notifyPdr(const Player &player, int resource);
            void notifyPgt(const Player &player, int resource);
            void notifyPdi(const Player &player);
            void notifyEnw(const Player &player, const Egg &egg);
            void notifyEbo(const Egg &egg);
            void notifyEdi(const Egg &egg);
            void sendSmg(const std::string &message);
    };
}

#endif
