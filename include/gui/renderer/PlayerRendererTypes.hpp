/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** PlayerRendererTypes.hpp
*/

#ifndef PLAYERRENDERER_TYPES_HPP_
    #define PLAYERRENDERER_TYPES_HPP_
    #include "state/Player.hpp"
    #include <raylib.h>

struct PlayerModel {
    Model model{};
    Vector3 modelScale = {1.0f, 1.0f, 1.0f};
    float baseYOffset = 0.0f;
    bool ready = false;
};

struct DrawContext {
    Vector3 base;
    Color teamColor;
    const PlayerModel *model;
    Orientation orientation;
    bool selected;
    bool inElevation;
};

#endif
