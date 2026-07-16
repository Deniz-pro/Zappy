/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** MinimapRenderer.cpp
*/

#include "gui/renderer/MinimapRenderer.hpp"
#include "gui/renderer/PlayerRenderer.hpp"
#include "gui/renderer/TileUtils.hpp"
#include <raylib.h>

Layout MinimapRenderer::computeLayout(const GameState &state) const
{
    Layout layout{};

    if (state.getMapWidth() == 0 || state.getMapHeight() == 0)
        return layout;
    layout.mapW = state.getMapWidth();
    layout.mapH = state.getMapHeight();
    layout.minimapW = layout.mapW * TILE_PX;
    layout.minimapH = layout.mapH * TILE_PX;
    layout.x = GetScreenWidth() - layout.minimapW - PADDING;
    layout.y = GetScreenHeight() - layout.minimapH - PADDING;
    return layout;
}

void MinimapRenderer::drawBackground(const Layout &layout) const
{
    DrawRectangle(layout.x - 2, layout.y - 2,
        layout.minimapW + 4, layout.minimapH + 4, Fade(BLACK, 0.8f));
}

void MinimapRenderer::drawTiles(const GameState &state,
    const Layout &layout) const
{
    for (int row = 0; row < layout.mapH; row++)
        for (int col = 0; col < layout.mapW; col++)
            DrawRectangle(layout.x + col * TILE_PX, layout.y + row * TILE_PX,
                TILE_PX, TILE_PX,
            TileUtils::getDominantColor(state.getTiles()[row][col]));
}

void MinimapRenderer::draw(const GameState &state) const
{
    Layout layout = computeLayout(state);

    if (layout.mapW == 0 || layout.mapH == 0)
        return;
    drawMinimap(state, layout);
}

void MinimapRenderer::drawMinimap(const GameState &state,
    const Layout &layout) const
{
    drawBackground(layout);
    drawTiles(state, layout);
    drawPlayers(state, layout);
}

MinimapRenderer::DotStyle MinimapRenderer::buildDotStyle(
    bool selected, const std::string &team) const
{
    if (selected)
        return {WHITE, 5, 2};
    return {PlayerRenderer::getTeamColor(team), 3, 1};
}

void MinimapRenderer::drawPlayerDot(const DotStyle &style,
    const Player &p, const Layout &layout) const
{
    DrawRectangle( layout.x + p.x * TILE_PX + TILE_PX / 2 - style.offset,
        layout.y + p.y * TILE_PX + TILE_PX / 2 - style.offset, style.size,
        style.size, style.color);
}

void MinimapRenderer::drawPlayers(const GameState &state,
    const Layout &layout) const
{
    bool selected;

    for (const auto &[id, p] : state.getPlayers()) {
        selected = (id == state.selectedPlayerId);
        drawPlayerDot(buildDotStyle(selected, p.team), p, layout);
    }
}
