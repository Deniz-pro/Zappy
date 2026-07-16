/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** PlayerRendererUtils.cpp
*/

#include "gui/renderer/PlayerRenderer.hpp"
#include <string>
#include <unordered_map>

Vector3 PlayerRenderer::playerWorldPos(const Player &player,
    float heightAboveTile)
{
    Vector3 pos;

    pos.x = player.x * TILE_SPACING;
    pos.y = TILE_HEIGHT + heightAboveTile;
    pos.z = player.y * TILE_SPACING;
    return pos;
}

Color PlayerRenderer::getTeamColor(const std::string &team)
{
    static const Color TEAM_COLORS[] = {
        RED, BLUE, GREEN, YELLOW, PURPLE, ORANGE, PINK, SKYBLUE
    };
    static std::unordered_map<std::string, int> colorMap;
    int idx;

    auto it = colorMap.find(team);
    if (it == colorMap.end()) {
        idx = static_cast<int>(colorMap.size()) % 8;
        colorMap[team] = idx;
        return TEAM_COLORS[idx];
    }
    return TEAM_COLORS[it->second];
}

int PlayerRenderer::findClickedPlayer(const GameState &state,
    const Camera3D &camera, Vector2 mouse) const
{
    Vector2 screen;
    float dx;
    float dy;

    for (const auto &[id, p] : state.getPlayers()) {
        screen = GetWorldToScreen(playerWorldPos(p,
            LABEL_Y_OFFSET), camera);
        dx = mouse.x - screen.x;
        dy = mouse.y - screen.y;
        if (dx * dx + dy * dy < SELECTION_RADIUS_SQ)
            return id;
    }
    return -1;
}
