/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** SkyboxRenderer.cpp
*/

#include "gui/renderer/SkyboxRenderer.hpp"
#include "gui/renderer/Config.hpp"
#include <raylib.h>
#include <rlgl.h>

static void drawStarPixel(Image &img, int x, int y, Color color)
{
    if (x < 0 || y < 0 || x >= img.width || y >= img.height)
        return;
    ImageDrawPixel(&img, x, y, color);
}

static Image createBaseImage(int size)
{
    return GenImageColor(size, size, {5, 5, 18, 255});
}

static void drawRandomStar(Image &img, int size)
{
    int x = GetRandomValue(0, size - 1);
    int y = GetRandomValue(0, size - 1);
    int brightness = GetRandomValue(190, 255);
    Color star = {static_cast<unsigned char>(brightness),
        static_cast<unsigned char>(brightness),
        static_cast<unsigned char>(brightness), 255};

    drawStarPixel(img, x, y, star);
    if (GetRandomValue(0, 99) > 94) {
        drawStarPixel(img, x + 1, y, star);
        drawStarPixel(img, x, y + 1, star);
    }
}

Image SkyboxRenderer::generateStarfieldImage(int size)
{
    Image img = createBaseImage(size);
    int starCount = size * size / 120;

    for (int i = 0; i < starCount; ++i)
        drawRandomStar(img, size);
    return img;
}

bool SkyboxRenderer::loadTexture()
{
    Image img = generateStarfieldImage(1024);

    _texture = LoadTextureFromImage(img);
    UnloadImage(img);
    if (!IsTextureReady(_texture)) {
        TraceLog(LOG_ERROR, "SkyboxRenderer: texture load failed");
        return false;
    }
    return true;
}

void SkyboxRenderer::load()
{
    if (_loaded)
        return;
    if (!loadTexture())
        return;
    _cube = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
    SetMaterialTexture(&_cube.materials[0], MATERIAL_MAP_ALBEDO, _texture);
    _loaded = true;
}

void SkyboxRenderer::draw(const Camera3D &camera) const
{
    if (!_loaded)
        return;
    rlDisableDepthMask();
    rlDisableDepthTest();
    rlDisableBackfaceCulling();
    DrawModel(_cube, camera.position, SKYBOX_SCALE, WHITE);
    rlEnableBackfaceCulling();
    rlEnableDepthTest();
    rlEnableDepthMask();
}

void SkyboxRenderer::unload()
{
    if (!_loaded)
        return;
    SetMaterialTexture(&_cube.materials[0], MATERIAL_MAP_ALBEDO, {});
    UnloadModel(_cube);
    UnloadTexture(_texture);
    _loaded = false;
}

SkyboxRenderer::~SkyboxRenderer()
{
    if (IsWindowReady())
        unload();
}
