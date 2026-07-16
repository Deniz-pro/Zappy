/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** MinimapRenderer.hpp
*/

#ifndef MINIMAPRENDERER_HPP_
    #define MINIMAPRENDERER_HPP_
    #include "state/GameState.hpp"
    #include <raylib.h>

inline constexpr int TILE_PX = 9;
inline constexpr int PADDING = 10;

struct Layout {
    int mapW;
    int mapH;
    int minimapW;
    int minimapH;
    int x;
    int y;
};

class MinimapRenderer {
    public:
        void draw(const GameState &state) const;
    private:
        struct DotStyle {
            Color color;
            int size;
            int offset;
        };
        Layout computeLayout(const GameState &state) const;
        void drawMinimap(const GameState &state, const Layout &layout) const;
        void drawBackground(const Layout &layout) const;
        void drawTiles(const GameState &state, const Layout &layout) const;
        void drawPlayers(const GameState &state, const Layout &layout) const;
        DotStyle buildDotStyle(bool selected,
            const std::string &team) const;
        void drawPlayerDot(const DotStyle &style,
            const Player &p, const Layout &layout) const;
};

#endif
