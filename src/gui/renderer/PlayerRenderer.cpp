/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** PlayerRenderer.cpp
*/

#include "gui/renderer/PlayerRenderer.hpp"

PlayerRenderer::~PlayerRenderer()
{
    if (IsWindowReady())
        unload();
}

void PlayerRenderer::ensureLoaded()
{
    if (_loaded)
        return;
    _model = loadPlayerAsset();
    _loaded = true;
}

void PlayerRenderer::draw(const GameState &state) const
{
    for (const auto &[id, p] : state.getPlayers())
        drawPlayer(buildContext(id, p, state.selectedPlayerId));
}

void PlayerRenderer::drawLabels(const GameState &state,
    const Camera3D &camera) const
{
    for (const auto &[id, p] : state.getPlayers()) {
        Vector2 screen = GetWorldToScreen(playerWorldPos(p,
            LABEL_Y_OFFSET), camera);
        DrawText(TextFormat("Player %d Level %d %s", id, p.level, p.team.c_str()),
            static_cast<int>(screen.x) - 6,
            static_cast<int>(screen.y), 11, WHITE);
    }
}

void PlayerRenderer::handleSelection(GameState &state,
    const Camera3D &camera) const
{
    Vector2 mouse = GetMousePosition();

    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return;
    if (mouse.x < 220)
        return;
    state.selectedPlayerId = findClickedPlayer(state, camera, mouse);
}
