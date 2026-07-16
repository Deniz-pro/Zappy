/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** GUICommands.hpp
*/

#ifndef GUICOMMANDS_HPP
    #define GUICOMMANDS_HPP
    constexpr const char *SBP = "sbp\n";
    #include "ACommands.hpp"
    #include <string>

namespace zappy {
    class GUICommands : public ACommands {
        void sendMsz();
        void sendBct(const std::string &pos);
        void sendMct();
        void sendTna();
        void sendPpo(const std::string &num);
        void sendPlv(const std::string &num);
        void sendPin(const std::string &num);
        void sendSgt();
        void sendSst(const std::string &freq);
        void sendSbp();
        void sendPnw();
        std::string buildBct(int x, int y);
        bool isValidInt(const std::string &s);
        void sendInitialState();
        bool parseBctArgs(const std::string &args, int &a, int &b);
        std::string removeHash(const std::string &s);
        public:
            GUICommands(int clientId, GameState &game_state, GUIEvents &events, Server *srv);
        protected:
            void init() override;
    };
}

#endif
