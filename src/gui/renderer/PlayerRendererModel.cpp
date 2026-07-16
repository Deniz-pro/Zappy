/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** PlayerRendererModel.cpp
*/

#include "gui/renderer/PlayerRenderer.hpp"
#include <raymath.h>

void PlayerRenderer::applyModelScale(PlayerModel &playerModel) const
{
    BoundingBox bounds = GetModelBoundingBox(playerModel.model);
    float rangeX = bounds.max.x - bounds.min.x;
    float rangeY = bounds.max.y - bounds.min.y;
    float rangeZ = bounds.max.z - bounds.min.z;
    float maxRange = rangeX > rangeY ? rangeX : rangeY;
    float scale;

    maxRange = maxRange > rangeZ ? maxRange : rangeZ;
    scale = (maxRange > 0) ? PLAYER_HEIGHT / maxRange : 1.0f;
    playerModel.modelScale = {scale, scale, scale};
    playerModel.baseYOffset = TILE_HEIGHT - bounds.min.y * scale;
}

PlayerModel PlayerRenderer::loadPlayerAsset() const
{
    PlayerModel playerModel{};

    playerModel.model = LoadModel(MODEL_PATH);
    if (!IsModelReady(playerModel.model)) {
        TraceLog(LOG_WARNING, "Failed to load model: %s", MODEL_PATH);
        return playerModel;
    }
    applyModelScale(playerModel);
    playerModel.ready = true;
    return playerModel;
}

void PlayerRenderer::unload()
{
    if (_model.ready)
        UnloadModel(_model.model);
    _model = {};
    _loaded = false;
}


void PlayerRenderer::drawTeamRing(Vector3 base, Color color) const
{
    Vector3 pos = {base.x, TILE_HEIGHT + 0.02f, base.z};

    DrawCircle3D(pos, 0.35f, {1.0f, 0.0f, 0.0f}, 90.0f, color);
}

static float orientationYaw(Orientation orientation)
{
    switch (orientation) {
        case Orientation::NORTH:
            return 0.0f;
        case Orientation::EAST: 
            return 270.0f;
        case Orientation::SOUTH:
            return 180.0f;
        case Orientation::WEST: 
            return 90.0f;
    }
    return 0.0f;
}

void PlayerRenderer::drawModel(const PlayerModel &playerModel,
    Vector3 base, Orientation orientation) const
{
    Vector3 position = {base.x, playerModel.baseYOffset, base.z};
    Matrix rotX = MatrixRotateX(-90.0f * DEG2RAD);
    Matrix rotY = MatrixRotateY(orientationYaw(orientation) * DEG2RAD);
    Matrix scale = MatrixScale(playerModel.modelScale.x,
        playerModel.modelScale.y, playerModel.modelScale.z);
    Model m = playerModel.model;

    m.transform = MatrixMultiply(MatrixMultiply(scale, rotX), rotY);
    DrawModel(m, position, 1.0f, WHITE);
}

DrawContext PlayerRenderer::buildContext(
    int id, const Player &p, int selectedId) const
{
    DrawContext ctx;

    ctx.base = playerWorldPos(p);
    ctx.teamColor = getTeamColor(p.team);
    ctx.model = _model.ready ? &_model : nullptr;
    ctx.orientation = p.orientation;
    ctx.selected = (id == selectedId);
    ctx.inElevation = p.in_elevation;
    return ctx;
}

void PlayerRenderer::drawPlayer(const DrawContext &ctx) const
{
    if (ctx.model) {
        drawModel(*ctx.model, ctx.base, ctx.orientation);
        drawTeamRing(ctx.base, ctx.teamColor);
    }
    Vector3 groundPos = {ctx.base.x, TILE_HEIGHT + 0.02f, ctx.base.z};
    if (ctx.inElevation)
        DrawCircle3D(groundPos, 0.45f, {1.0f, 0.0f, 0.0f}, 90.0f, PURPLE);
    if (ctx.selected)
        DrawCircle3D(groundPos, 0.85f, {1.0f, 0.0f, 0.0f}, 90.0f, WHITE);
}
