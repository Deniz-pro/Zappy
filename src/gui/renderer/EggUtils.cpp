/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** EggUtils.cpp
*/

#include "gui/renderer/EggUtils.hpp"
#include "gui/renderer/PlayerRenderer.hpp"
#include "gui/renderer/Config.hpp"

static EggVisual makeIdleVisual(const zappy::Egg &egg)
{
    EggVisual visual;

    if (egg.team.empty())
        visual.color = WHITE;
    else
        visual.color = PlayerRenderer::getTeamColor(egg.team);
    return visual;
}

static EggVisual makeHatchingVisual(const zappy::Egg &egg)
{
    EggVisual visual;
    float progress = 1.0f - egg.animTimer / HATCH_DURATION;

    visual.color = GREEN;
    visual.scale = 1.0f + progress * 0.4f;
    return visual;
}

static EggVisual makeDyingVisual(const zappy::Egg &egg)
{
    EggVisual visual;
    float progress = egg.animTimer / DIE_DURATION;

    visual.color = RED;
    visual.alpha = progress;
    visual.scale = 0.6f + 0.4f * progress;
    return visual;
}

EggVisual EggUtils::computeVisual(const zappy::Egg &egg)
{
    if (egg.anim == zappy::EggAnim::HATCHING)
        return makeHatchingVisual(egg);
    if (egg.anim == zappy::EggAnim::DYING)
        return makeDyingVisual(egg);
    return makeIdleVisual(egg);
}
