/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** PlanetRenderer.cpp
*/

#include "gui/renderer/PlanetRenderer.hpp"
#include <rlgl.h>
#include <cmath>
#include <algorithm>

static float pHash(int x, int y)
{
    int n = x + y * 57;

    n = (n << 13) ^ n;
    return 1.0f - static_cast<float>((n * (n * n * 15731 + 789221) + 1376312589) &
    0x7fffffff) / 1073741824.0f;
}

static float pNoise(float x, float y)
{
    int ix = static_cast<int>(std::floor(x));
    int iy = static_cast<int>(std::floor(y));
    float fx = x - std::floor(x);
    float fy = y - std::floor(y);
    float u = fx * fx * (3.0f - 2.0f * fx);
    float v = fy * fy * (3.0f - 2.0f * fy);
    float a = pHash(ix, iy);
    float b = pHash(ix + 1, iy);
    float c = pHash(ix, iy + 1);
    float d = pHash(ix + 1, iy + 1);

    return a + (b - a) * u + (c - a) * v + (a - b - c + d) * u * v;
}

static float pFbm(float x, float y)
{
    float v = 0.0f;
    float a = 0.5f;

    for (int i = 0; i < 6; i++) {
        v += a * pNoise(x, y);
        x *= 2.1f;
        y *= 2.1f;
        a *= 0.5f;
    }
    return v;
}

static Image genPlanetImage()
{
    const int W = 512;
    const int H = 256;
    Image img = GenImageColor(W, H, Color{0, 80, 200, 255});
    Color *px = static_cast<Color *>(img.data);
    float n = 0.0f;
    Color c = {};

    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            n = pFbm(x * 4.5f / W, y * 4.5f / H);
            if (n < 0.04f)
                c = {0, 50, 180, 255};
            else if (n < 0.08f)
                c = {0, 90, 220, 255};
            else if (n < 0.11f)
                c = {220, 200, 120, 255};
            else if (n < 0.50f)
                c = {30, 160, 40, 255};
            else if (n < 0.72f)
                c = {20, 120, 20, 255};
            else
                c = {130, 110, 90, 255};
            px[y * W + x] = c;
        }
    }
    return img;
}

PlanetRenderer::~PlanetRenderer()
{
    if (!_loaded || !IsWindowReady())
        return;
    UnloadModel(_model);
    UnloadTexture(_tex);
}

void PlanetRenderer::draw(Vector3 center, float radius) const
{
    float angle = static_cast<float>(GetTime()) * 8.0f;

    if (!_loaded) {
        Image img = genPlanetImage();
        _tex = LoadTextureFromImage(img);
        UnloadImage(img);
        SetTextureFilter(_tex, TEXTURE_FILTER_BILINEAR);
        _model = LoadModelFromMesh(GenMeshSphere(radius, 64, 64));
        _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = _tex;
        _loaded = true;
    }
    DrawModelEx(_model, center, {0.0f, 1.0f, 0.0f}, angle, {1.0f, 1.0f, 1.0f}, WHITE);
}

void PlanetRenderer::drawOrbitalRing(Vector3 center, float radius) const
{
    float ringAngle = static_cast<float>(GetTime()) * -4.0f;
    float ringInner = radius * 1.15f;
    float ringOuter = radius * 1.45f;
    int ringLines = 18;
    float ringStep = (ringOuter - ringInner) / ringLines;
    float r = 0.0f;
    unsigned char alpha = 0;

    rlPushMatrix();
    rlTranslatef(center.x, center.y, center.z);
    rlRotatef(ringAngle, 0.0f, 1.0f, 0.0f);
    rlRotatef(20.0f, 1.0f, 0.0f, 0.0f);
    BeginBlendMode(BLEND_ALPHA);
    for (int i = 0; i <= ringLines; i++) {
        r = ringInner + i * ringStep;
        alpha = static_cast<unsigned char>(100 + 80 * (1.0f - static_cast<float>(i) /
        ringLines));
        DrawCircle3D({0.0f, 0.0f, 0.0f}, r,
            {1.0f, 0.0f, 0.0f}, 90.0f, Color{200, 165, 80, alpha});
    }
    EndBlendMode();
    rlPopMatrix();
}

static Vector3 starOrigin(Vector3 center, float dist, float ang, float elev)
{
    return {
        center.x + dist * cosf(ang) * cosf(elev),
        center.y + dist * sinf(elev),
        center.z + dist * sinf(ang) * cosf(elev)
    };
}

static Vector3 starTail(Vector3 origin, float ang, float streakLen, float progress)
{
    float dx = -cosf(ang);
    float dy = -0.2f;
    float dz = -sinf(ang);
    float len = sqrtf(dx * dx + dy * dy + dz * dz);
    dx /= len; dy /= len; dz /= len;
    return {
        origin.x + dx * streakLen * progress,
        origin.y + dy * streakLen * progress,
        origin.z + dz * streakLen * progress
    };
}

static void drawOneStar(Vector3 center, float dist, float streakLen,
    float ang, float elev, float progress)
{
    float alpha = 1.0f - progress;

    Vector3 origin = starOrigin(center, dist, ang, elev);
    Vector3 tail = starTail(origin, ang, streakLen, progress);
    DrawLine3D(origin, tail, Fade(WHITE, alpha));
}

void PlanetRenderer::drawShootingStars(Vector3 center, float radius) const
{
    static constexpr int N = 4;
    static constexpr float PERIOD = 5.0f;
    static constexpr float DURATION = 0.45f;
    static const float OFFSETS[N] = {0.0f, 1.4f, 2.9f, 4.2f};
    static const float ANGLES[N] = {0.5f, 2.1f, 3.8f, 5.4f};
    static const float ELEVS[N] = {0.55f, 0.75f, 0.50f, 0.65f};
    float t = static_cast<float>(GetTime());
    float dist = radius * 2.8f;
    float streakLen = radius * 0.7f;
    float tLocal = 0.0f;

    BeginBlendMode(BLEND_ADDITIVE);
    for (int s = 0; s < N; s++) {
        tLocal = fmodf(t + OFFSETS[s], PERIOD);
        if (tLocal > DURATION)
            continue;
        drawOneStar(center, dist, streakLen,
            ANGLES[s], ELEVS[s], tLocal / DURATION);
    }
    EndBlendMode();
}
