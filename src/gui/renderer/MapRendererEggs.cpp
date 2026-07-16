/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** MapRendererEggs.cpp
*/

#include "gui/renderer/MapRenderer.hpp"
#include <raylib.h>

void MapRenderer::drawEggMesh(int x, int y, const EggVisual &visual) const
{
    Vector3 pos = {x * TILE_SPACING, TILE_HEIGHT + 0.2f * visual.scale, y * TILE_SPACING};
    Color color = Fade(visual.color, visual.alpha);
    float r = 0.25f * visual.scale;

    DrawSphere(pos, r, color);
    DrawSphereWires(pos, r + 0.01f, 6, 6, BLACK);
}

void MapRenderer::drawEggs(const GameState &state) const
{
    BeginBlendMode(BLEND_ALPHA);
    for (const auto &[id, egg] : state.getEggs())
        drawEggMesh(egg.x, egg.y, EggUtils::computeVisual(egg));
    EndBlendMode();
}
