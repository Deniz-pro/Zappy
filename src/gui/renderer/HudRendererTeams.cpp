/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** HudRendererTeams.cpp
*/

#include "gui/renderer/HudRenderer.hpp"
#include "gui/renderer/PlayerRenderer.hpp"
#include "gui/renderer/Config.hpp"
#include "gui/renderer/TileUtils.hpp"
#include "state/Player.hpp"
#include "server/Elevation.hpp"
#include <raylib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

static bool isTeamSelected(const GameState &state, const std::string &team)
{
    if (state.selectedPlayerId == -1)
        return false;
    auto it = state.getPlayers().find(state.selectedPlayerId);
    return it != state.getPlayers().end() && it->second.team == team;
}

static int countTeamPlayers(const std::string &team,
    const std::unordered_map<int, Player> &players)
{
    int count = 0;

    for (const auto &[id, p] : players)
        if (p.team == team)
            count++;
    return count;
}

static void drawTeamEntry(const GameState &state,
    const std::string &team, int count, int x, int y)
{
    bool selected = isTeamSelected(state, team);
    Color color = PlayerRenderer::getTeamColor(team);
    Color textColor = selected ? YELLOW : WHITE;

    DrawRectangle(x, y, HUD_BOX_SIZE, HUD_BOX_SIZE, color);
    if (selected)
        DrawRectangleLines(x - 1, y - 1, HUD_BOX_SIZE + 2,
            HUD_BOX_SIZE + 2, YELLOW);
    DrawText(TextFormat("%s (%d)", team.c_str(), count),
        x + HUD_BOX_SIZE + 6, y, HUD_FONT_SIZE, textColor);
}

std::vector<const Player *> HudRenderer::getSortedTeamPlayers(
    const GameState &state, const std::string &team) const
{
    std::vector<const Player *> players;

    for (const auto &[id, p] : state.getPlayers())
        if (p.team == team)
            players.push_back(&p);
    std::ranges::sort(players, {}, &Player::id);
    return players;
}

void HudRenderer::drawPlayerEntry(
    const Player &p, int selectedId, int x, int &y) const
{
    bool selected = (p.id == selectedId);
    Color color = selected ? YELLOW : LIGHTGRAY;

    if (selected)
        DrawRectangle(x, y - 1, 200, HUD_FONT_SIZE + 2, Fade(YELLOW, 0.15f));
    DrawText(TextFormat("  Player %d Level %d", p.id, p.level),
        x + 4, y, HUD_FONT_SIZE - 2, color);
    y += HUD_FONT_SIZE + 2;
}

void HudRenderer::drawPlayerList(const GameState &state,
    const std::string &team, int x, int &y) const
{
    std::vector<const Player *> players = getSortedTeamPlayers(state, team);
    int selectedId = state.selectedPlayerId;

    for (const Player *p : players)
        drawPlayerEntry(*p, selectedId, x, y);
}

void HudRenderer::drawTeamRow(const GameState &state,
    const std::string &team, int x, int &y) const
{
    const auto &players = state.getPlayers();
    bool expanded = (team == state.selectedTeam);

    drawTeamEntry(state, team, countTeamPlayers(team, players), x, y);
    if (expanded)
        DrawText("<", x + 165, y - 3, HUD_FONT_SIZE * 2, YELLOW);
    y += HUD_BOX_SIZE + 4;
    if (expanded)
        drawPlayerList(state, team, x, y);
}

void HudRenderer::drawTeams(const GameState &state) const
{
    const auto &teams = state.getTeams();
    int x = 10;
    int y = state.getMapWidth() > 0 ? FREQ_TEAM_Y : 70;

    if (teams.empty())
        return;
    DrawText("Teams:", x, y, HUD_FONT_SIZE, WHITE);
    y += HUD_FONT_SIZE + 4;
    for (const auto &team : teams)
        drawTeamRow(state, team, x, y);
}

static void drawInventoryItems(const Player &p, int x, int y)
{
    std::string s;

    for (int i = 0; i < 7; i++) {
        s = std::string(RESOURCE_FULL_NAMES[i]) +
            ": " + std::to_string(p.inventory[i]);
        DrawRectangle(x, y, HUD_BOX_SIZE, HUD_BOX_SIZE, RESOURCE_COLORS[i]);
        DrawText(s.c_str(), x + HUD_BOX_SIZE + 4, y, HUD_FONT_SIZE, WHITE);
        y += HUD_FONT_SIZE + 2;
    }
}

static int computeProgressHeight(const Player &p)
{
    int count = 0;

    if (p.level >= 8)
        return 0;
    const auto &req = zappy::ELEVATION_TABLE[p.level - 1];
    const int needed[6] = {req.linemate, req.deraumere, req.sibur,
        req.mendiane, req.phiras, req.thystame};
    for (int i = 0; i < 6; i++)
        if (needed[i] > 0) count++;
    return HUD_FONT_SIZE + 4 + count * 12;
}

static void drawProgressBars(const Player &p, const int needed[6], int x, int y)
{
    int have = 0;
    float ratio = 0.0f;

    for (int i = 0; i < 6; i++) {
        if (needed[i] == 0)
            continue;
        have = std::min(p.inventory[i + 1], needed[i]);
        ratio = static_cast<float>(have) / needed[i];
        DrawRectangle(x, y, 100, 8, DARKGRAY);
        DrawRectangle(x, y, static_cast<int>(100.0f * ratio), 8,
        RESOURCE_SHAPE_COLORS[i + 1]);
        DrawText(TextFormat("%s %d/%d", RESOURCE_FULL_NAMES[i + 1], have, needed[i]),
        x + 104, y - 1, HUD_FONT_SIZE - 2, WHITE);
        y += 12;
    }
}

static void drawLevelProgress(const Player &p, int x, int y)
{
    if (p.level >= 8)
        return;
    const auto &req = zappy::ELEVATION_TABLE[p.level - 1];
    const int needed[6] = {req.linemate, req.deraumere, req.sibur,
        req.mendiane, req.phiras, req.thystame};

    DrawText(TextFormat("Next lvl %d (x%d players):", p.level + 1, req.players),
        x, y, HUD_FONT_SIZE, GOLD);
    drawProgressBars(p, needed, x, y + HUD_FONT_SIZE + 4);
}

static void drawInventoryPanel(const Player &p, int x, int y)
{
    int progressH = computeProgressHeight(p);
    int panelH = 8 * (HUD_FONT_SIZE + 2) + 8 + progressH;
    int itemsY = y + HUD_FONT_SIZE + 4;

    DrawRectangle(x - 4, y - 4, HUD_LEGEND_OFFSET + 60, panelH, Fade(BLACK, 0.7f));
    DrawText(TextFormat("Player %d %s Level %d", p.id, p.team.c_str(), p.level),
    x, y, HUD_FONT_SIZE, WHITE);
    drawInventoryItems(p, x, itemsY);
    drawLevelProgress(p, x, itemsY + 7 * (HUD_FONT_SIZE + 2) + 2);
}

void HudRenderer::drawInventory(const GameState &state) const
{
    int x = 10;
    int progressH = 0;
    int panelH = 0;
    int y = 0;
    auto it = state.getPlayers().find(state.selectedPlayerId);

    if (state.selectedPlayerId == -1)
        return;
    if (it == state.getPlayers().end())
        return;
    progressH = computeProgressHeight(it->second);
    panelH = 8 * (HUD_FONT_SIZE + 2) + 8 + progressH;
    y = GetScreenHeight() - panelH - 14;
    drawInventoryPanel(it->second, x, y);
}
