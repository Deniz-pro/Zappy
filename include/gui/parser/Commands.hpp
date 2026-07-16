/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Commands.hpp
*/

#ifndef COMMANDS_HPP
    #define COMMANDS_HPP
    #include "state/GameState.hpp"
    #include <sstream>

class Commands {
    public:
        static void msz(std::istringstream &stream, GameState &state);
        static void bct(std::istringstream &stream, GameState &state);
        static void tna(std::istringstream &stream, GameState &state);
        static void pnw(std::istringstream &stream, GameState &state);
        static void ppo(std::istringstream &stream, GameState &state);
        static void plv(std::istringstream &stream, GameState &state);
        static void pin(std::istringstream &stream, GameState &state);
        static void pdi(std::istringstream &stream, GameState &state);
        static void pic(std::istringstream &stream, GameState &state);
        static void pie(std::istringstream &stream, GameState &state);
        static void pbc(std::istringstream &stream, GameState &state);
        static void enw(std::istringstream &stream, GameState &state);
        static void ebo(std::istringstream &stream, GameState &state);
        static void edi(std::istringstream &stream, GameState &state);
        static void pgt(std::istringstream &stream, GameState &state);
        static void pdr(std::istringstream &stream, GameState &state);
        static void seg(std::istringstream &stream, GameState &state);
        static void sgt(std::istringstream &stream, GameState &state);
        static void smg(std::istringstream &stream, GameState &state);
        static void pfk(std::istringstream &stream, GameState &state);
        static void pex(std::istringstream &stream, GameState &state);
        static void sst(std::istringstream &stream, GameState &state);
        static void suc(std::istringstream &stream, GameState &state);
        static void sbp(std::istringstream &stream, GameState &state);
    private:
};

#endif
