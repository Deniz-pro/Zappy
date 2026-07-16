/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** HudRenderer.cpp
*/

#include "gui/renderer/HudRenderer.hpp"
#include "gui/renderer/PlayerRenderer.hpp"
#include "gui/renderer/Config.hpp"
#include "gui/renderer/TileUtils.hpp"
#include "state/Player.hpp"
#include <raylib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

int HudRenderer::computeExpandedTeamHeight(
    const GameState &state, const std::string &team) const
{
    int h = 0;

    for (const auto &[id, p] : state.getPlayers())
        if (p.team == team)
            h += HUD_FONT_SIZE + 2;
    return h;
}

int HudRenderer::computeLeftPanelHeight(const GameState &state) const
{
    int h = FREQ_TEAM_Y + HUD_FONT_SIZE + 4 + 8;

    for (const auto &team : state.getTeams()) {
        h += HUD_BOX_SIZE + 4;
        if (team == state.selectedTeam)
            h += computeExpandedTeamHeight(state, team);
    }
    return h;
}

void HudRenderer::drawLeftBackground(const GameState &state) const
{
    int h = computeLeftPanelHeight(state);

    DrawRectangleRounded({0, 0, 220, (float)h}, 0.08f, 6, Fade(BLACK, 0.45f));
}

void HudRenderer::drawRightBackground() const
{
    int x = GetScreenWidth() - HUD_LEGEND_OFFSET - 6;
    int h = HUD_FONT_SIZE + 4 + 7 * (HUD_BOX_SIZE + 4) + 6;

    DrawRectangleRounded({(float)x, 4, HUD_LEGEND_OFFSET + 4, (float)h},
        0.08f, 6, Fade(BLACK, 0.45f));
}

void HudRenderer::drawFps() const
{
    DrawFPS(10, 10);
}

void HudRenderer::drawMapSize(const GameState &state) const
{
    std::string text;

    if (state.getMapWidth() == 0 || state.getMapHeight() == 0)
        return;
    text = "Map: " + std::to_string(state.getMapWidth()) +
        "x" + std::to_string(state.getMapHeight());
    DrawText(text.c_str(), 10, 40, 20, WHITE);
    if (state.freq > 0)
        DrawText(TextFormat("Freq: %d", state.freq), 10, 65, HUD_FONT_SIZE, WHITE);
}

void HudRenderer::drawLegend() const
{
    int x = GetScreenWidth() - HUD_LEGEND_OFFSET;
    int y = 10;
    int boxSize = HUD_BOX_SIZE;
    int fontSize = HUD_FONT_SIZE;

    DrawText("Resources:", x, y, fontSize, WHITE);
    y += fontSize + 4;
    for (int i = 0; i < 7; i++) {
        DrawRectangle(x, y, boxSize, boxSize, RESOURCE_COLORS[i]);
        DrawText(RESOURCE_FULL_NAMES[i], x + boxSize + 6, y, fontSize, WHITE);
        y += boxSize + 4;
    }
}

void HudRenderer::drawWinner(const GameState &state) const
{
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    std::string text = "Team " + state.getWinner() + " wins!";
    int fontSize = 60;
    int textW = MeasureText(text.c_str(), fontSize);

    if (state.getWinner().empty())
        return;
    DrawRectangle(0, 0, w, h, Fade(BLACK, 0.7f));
    DrawText(text.c_str(), (w - textW) / 2, (h - fontSize) / 2, fontSize, GOLD);
}

void HudRenderer::drawServerMessage(const GameState &state) const
{
    int fontSize = 18;
    int y = GetScreenHeight() - fontSize - 10;

    if (state.getServerMessage().empty())
        return;
    DrawRectangle(0, y - 4, GetScreenWidth(), fontSize + 8, Fade(BLACK, 0.6f));
    DrawText(state.getServerMessage().c_str(), 10, y, fontSize, SKYBLUE);
}

void HudRenderer::drawFreqControls(const GameState &state) const
{
    static const char *labels[] = {"<<", "<", ">", ">>"};
    int bx = 0;
    int tw = 0;

    if (state.getMapWidth() == 0)
        return;
    for (int i = 0; i < 4; i++) {
        bx = FREQ_BTN_X + i * (FREQ_BTN_W + FREQ_BTN_GAP);
        DrawRectangle(bx, FREQ_BTN_Y, FREQ_BTN_W, FREQ_BTN_H, DARKGRAY);
        DrawRectangleLines(bx, FREQ_BTN_Y, FREQ_BTN_W, FREQ_BTN_H, GRAY);
        tw = MeasureText(labels[i], HUD_FONT_SIZE);
        DrawText(labels[i], bx + (FREQ_BTN_W - tw) / 2,
            FREQ_BTN_Y + (FREQ_BTN_H - HUD_FONT_SIZE) / 2,
            HUD_FONT_SIZE, WHITE);
    }
}

void HudRenderer::draw(const GameState &state) const
{
    drawLeftBackground(state);
    drawRightBackground();
    drawFps();
    drawMapSize(state);
    drawFreqControls(state);
    drawLegend();
    drawTeams(state);
    drawInventory(state);
    drawServerMessage(state);
    drawWinner(state);
}
