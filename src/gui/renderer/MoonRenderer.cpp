/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** MoonRenderer.cpp
*/

#include "gui/renderer/MoonRenderer.hpp"
#include <cmath>

void MoonRenderer::draw(Vector3 planetCenter, float planetRadius) const
{
    float angle = static_cast<float>(GetTime()) * -0.4f;
    float orbitR = planetRadius * 1.75f;
    float moonR = planetRadius * 0.13f;
    Vector3 moonPos = {
        planetCenter.x + orbitR * cosf(angle),
        planetCenter.y,
        planetCenter.z + orbitR * sinf(angle)
    };

    DrawSphereEx(moonPos, moonR, 16, 16, Color{175, 170, 165, 255});
}
