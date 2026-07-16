/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** MapRendererEffects.cpp
*/

#include "gui/renderer/MapRenderer.hpp"
#include <raylib.h>
#include <string>
#include <deque>
#include <vector>

void MapRenderer::drawIncantations(const GameState &state) const
{
    Vector3 tilePos = {};
    Color color = {128, 0, 255, 80};

    BeginBlendMode(BLEND_ALPHA);
    for (const auto &entry : state.getIncantations()) {
        const IncantationState &inc = entry.second;
        tilePos = {inc.x * TILE_SPACING, TILE_HEIGHT + 0.01f,
            inc.y * TILE_SPACING};
        color = {128, 0, 255, 80};
        if (!inc.active) {
            color = inc.flashSuccess ? Color{255, 215, 0, 180}
            : Color{255, 0, 0, 180};
        }
        DrawCube(tilePos, TILE_SIZE, 0.02f, TILE_SIZE, color);
    }
    EndBlendMode();
}

void MapRenderer::drawExpulsions(const GameState &state) const
{
    int x = 0;
    int y = 0;
    float alpha = 0.0f;
    Vector3 tilePos = {};

    BeginBlendMode(BLEND_ALPHA);
    for (const auto &[key, timer] : state.getExpulsions()) {
        x = key % state.getMapWidth();
        y = key / state.getMapWidth();
        alpha = timer / FLASH_DURATION;
        tilePos = {x * TILE_SPACING, TILE_HEIGHT + 0.02f, y * TILE_SPACING};
        DrawCube(tilePos, TILE_SIZE, 0.02f, TILE_SIZE,
            Color{255, 140, 0, static_cast<unsigned char>(180 * alpha)});
    }
    EndBlendMode();
}

void MapRenderer::drawBroadcastBubble(const std::string &label,
    int centerX, int y, float alpha) const
{
    static constexpr int PAD = 4;
    int textW = MeasureText(label.c_str(), BROADCAST_FONT_SIZE);
    int boxW = textW + PAD * 2;
    int boxH = BROADCAST_FONT_SIZE + PAD * 2;
    int boxX = centerX - boxW / 2;

    DrawRectangle(boxX, y - PAD, boxW, boxH, Fade(BLACK, 0.6f * alpha));
    DrawText(label.c_str(), centerX - textW / 2, y,
    BROADCAST_FONT_SIZE, Fade(YELLOW, alpha));
}

void MapRenderer::drawBroadcastStack(int playerId,
    const std::deque<BroadcastMessage> &msgs, Vector2 screen) const
{
    int stackIdx = 0;
    float alpha = 0.0f;
    std::string label = "";
    int bubbleY = 0;

    for (int i = static_cast<int>(msgs.size()) - 1; i >= 0; i--) {
        alpha = msgs[i].timer / BROADCAST_DURATION;
        label = "#" + std::to_string(playerId) + ": " + msgs[i].text;
        bubbleY = static_cast<int>(screen.y) - stackIdx * BROADCAST_LINE_HEIGHT;
        drawBroadcastBubble(label, static_cast<int>(screen.x), bubbleY, alpha);
        stackIdx++;
    }
}

void MapRenderer::drawPlayerBroadcasts(int playerId,
    const std::deque<BroadcastMessage> &deque, Vector2 screen) const
{
    drawBroadcastStack(playerId, deque, screen);
}

void MapRenderer::drawBroadcasts(const GameState &state) const
{
    const auto &broadcasts = state.getBroadcasts();
    const auto &players = state.getPlayers();
    auto it = players.end();
    Vector3 worldPos = {};
    Vector2 screen = {};

    for (const auto &[id, queue] : broadcasts) {
        if (queue.empty())
            continue;
        it = players.find(id);
        if (it == players.end())
            continue;
        worldPos = {it->second.x * TILE_SPACING, TILE_HEIGHT + 1.6f,
            it->second.y * TILE_SPACING};
        screen = GetWorldToScreen(worldPos, _camera->getCamera());
        drawPlayerBroadcasts(id, queue, screen);
    }
}
