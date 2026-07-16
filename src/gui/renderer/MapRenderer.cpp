/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** MapRenderer.cpp
*/

#include "gui/renderer/MapRenderer.hpp"
#include "gui/renderer/TileUtils.hpp"
#include <raylib.h>
#include <memory>
#include <string>
#include <algorithm>

void MapRenderer::initCamera(int mapW, int mapH)
{
    CameraController::Params params;
    float centerX = (mapW - 1) * TILE_SPACING / 2.0f;
    float centerZ = (mapH - 1) * TILE_SPACING / 2.0f;
    float distance = static_cast<float>(mapW > mapH ? mapW : mapH);

    distance *= TILE_SPACING * 1.5f;
    params.centerX = centerX;
    params.centerZ = centerZ;
    params.distance = distance;
    _camera = std::make_unique<CameraController>(params);
}

void MapRenderer::drawTile(int col, int row, const Tile &tile) const
{
    Vector3 position = {col * TILE_SPACING, 0.0f, row * TILE_SPACING};

    DrawCube(position, TILE_SIZE, TILE_HEIGHT,
        TILE_SIZE, TileUtils::getDominantColor(tile));
    DrawCubeWires(position, TILE_SIZE, TILE_HEIGHT, TILE_SIZE, BLACK);
}

void MapRenderer::drawResourceShape(int type, Vector3 base,
    Color color, int count) const
{
    float s = 0.09f + std::min(count - 1, 4) * 0.015f;
    Vector3 top = {base.x, base.y + s * 2, base.z};

    switch (type) {
        case 0:
            DrawSphere({base.x, base.y + s * 1.4f, base.z}, s * 1.4f, color);
            break;
        case 1:
            DrawCube({base.x, base.y + s, base.z}, s * 2, s * 2, s * 2, color);
            break;
        case 2:
            DrawCylinder(base, s, s, s * 2, 8, color);
            break;
        case 3:
            DrawCylinder(base, 0.0f, s * 1.5f, s * 2, 8, color);
            break;
        case 4:
            DrawCapsule(base, top, s * 0.7f, 8, 4, color);
            break;
        case 5:
            DrawCylinder(base, s * 1.5f, s * 1.5f, s * 0.4f, 16, color);
            break;
        case 6:
            DrawCylinder(base, 0.0f, s * 1.5f, s * 2, 8, color);
            DrawCylinder({base.x, base.y + s * 2, base.z},
                s * 1.5f, 0.0f, s * 2, 8, color);
            break;
        default:
            break;
    }
}

void MapRenderer::drawResourceMesh(const Tile &tile, int col, int row) const
{
    float cx = col * TILE_SPACING;
    float cz = row * TILE_SPACING;

    for (int i = 0; i < 7; i++) {
        if (tile.resources[i] == 0)
            continue;
        Vector3 base = {cx + RESOURCE_OFFSETS[i].x, TILE_HEIGHT, cz + RESOURCE_OFFSETS[i].y};
        drawResourceShape(i, base, RESOURCE_SHAPE_COLORS[i], tile.resources[i]);
    }
}

void MapRenderer::drawResourceMeshes(const GameState &state) const
{
    for (int row = 0; row < state.getMapHeight(); row++)
        for (int col = 0; col < state.getMapWidth(); col++)
            drawResourceMesh(state.getTiles()[row][col], col, row);
}

void MapRenderer::drawTiles(const GameState &state) const
{
    for (int row = 0; row < state.getMapHeight(); row++)
        for (int col = 0; col < state.getMapWidth(); col++)
            drawTile(col, row, state.getTiles()[row][col]);
}

void MapRenderer::drawScene(GameState &state)
{
    BeginMode3D(_camera->getCamera());
    _skybox.draw(_camera->getCamera());
    drawTiles(state);
    drawResourceMeshes(state);
    drawEggs(state);
    drawIncantations(state);
    drawExpulsions(state);
    _playerRenderer.draw(state);
    EndMode3D();
    _playerRenderer.drawLabels(state, _camera->getCamera());
    drawBroadcasts(state);
}

void MapRenderer::handleSelection(GameState &state)
{
    if (!_camera)
        return;
    _playerRenderer.handleSelection(state, _camera->getCamera());
}

void MapRenderer::draw(GameState &state)
{
    if (state.getMapWidth() == 0 || state.getMapHeight() == 0)
        return;
    _playerRenderer.ensureLoaded();
    _skybox.load();
    if (!_camera || state.isMapChanged()) {
        initCamera(state.getMapWidth(), state.getMapHeight());
        state.resetMapChanged();
    }
    _camera->update();
    drawScene(state);
}
