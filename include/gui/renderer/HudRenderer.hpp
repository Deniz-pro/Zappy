/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** HudRenderer.hpp
*/

#ifndef HUDRENDERER_HPP_
    #define HUDRENDERER_HPP_
    #include "state/GameState.hpp"
    #include <vector>

inline constexpr const char *RESOURCE_FULL_NAMES[7] = {
    "Food", "Linemate", "Deraumere", "Sibur", "Mendiane", "Phiras", "Thystame"
};

class HudRenderer {
    public:
        void draw(const GameState &state) const;
    private:
        void drawFps() const;
        void drawMapSize(const GameState &state) const;
        void drawLegend() const;
        void drawTeams(const GameState &state) const;
        void drawTeamRow(const GameState &state, const std::string &team,
            int x, int &y) const;
        void drawPlayerList(const GameState &state,
            const std::string &team, int x, int &y) const;
        void drawPlayerEntry(const Player &p, int selectedId, int x, int &y) const;
        std::vector<const Player *> getSortedTeamPlayers(
            const GameState &state, const std::string &team) const;
        void drawLeftBackground(const GameState &state) const;
        void drawRightBackground() const;
        int computeLeftPanelHeight(const GameState &state) const;
        int computeExpandedTeamHeight(const GameState &state,
            const std::string &team) const;
        void drawInventory(const GameState &state) const;
        void drawFreqControls(const GameState &state) const;
        void drawWinner(const GameState &state) const;
        void drawServerMessage(const GameState &state) const;
};

#endif
