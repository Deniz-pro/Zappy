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
    Vector2 screen = {0};
    const char *label = nullptr;
    Color teamColor = WHITE;
    int fontSize = 11;
    int textW = 0;
    int bx = 0;
    int by = 0;

    for (const auto &[id, p] : state.getPlayers()) {
        screen = GetWorldToScreen(playerWorldPos(p, LABEL_Y_OFFSET), camera);
        label = TextFormat("#%d L%d %s", id, p.level, p.team.c_str());
        teamColor = getTeamColor(p.team);
        textW = MeasureText(label, fontSize);
        bx = static_cast<int>(screen.x) - textW / 2 - 4;
        by = static_cast<int>(screen.y) - 2;
        DrawRectangle(bx, by, textW + 8, fontSize + 4, Fade(BLACK, 0.6f));
        DrawRectangleLines(bx, by, textW + 8, fontSize + 4, Fade(teamColor, 0.85f));
        DrawText(label, bx + 4, by + 2, fontSize, WHITE);
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
