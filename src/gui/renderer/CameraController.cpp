/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** CameraController.cpp
*/

#include "gui/renderer/CameraController.hpp"
#include "gui/renderer/Config.hpp"
#include <cmath>

CameraController::CameraController(const Params &params)
: _camera({}), _yaw(0.0f), _pitch(0.8f), _distance(params.distance),
_centerX(params.centerX), _centerZ(params.centerZ)
{
    _camera.up = {0.0f, 1.0f, 0.0f};
    _camera.fovy = 45.0f;
    _camera.projection = CAMERA_PERSPECTIVE;
    applySpherical();
}

const Camera3D &CameraController::getCamera() const
{
    return _camera;
}

void CameraController::applySpherical()
{
    float x = _centerX + _distance * cosf(_pitch) * sinf(_yaw);
    float y = _distance * sinf(_pitch);
    float z = _centerZ + _distance * cosf(_pitch) * cosf(_yaw);

    _camera.position = {x, y, z};
    _camera.target = {_centerX, 0.0f, _centerZ};
}

void CameraController::handleZoom(float dt)
{
    float wheel = GetMouseWheelMove();

    if (wheel != 0.0f)
        _distance -= wheel * ZOOM_SPEED * 10.0f * dt;
    if (IsKeyDown(KEY_UP))
        _distance -= ZOOM_SPEED * 20.0f * dt;
    if (IsKeyDown(KEY_DOWN))
        _distance += ZOOM_SPEED * 20.0f * dt;
}

void CameraController::handleRotation(float dt)
{
    Vector2 delta;

    if (IsKeyDown(KEY_LEFT))
        _yaw -= ROTATE_SPEED * 30.0f * dt;
    if (IsKeyDown(KEY_RIGHT))
        _yaw += ROTATE_SPEED * 30.0f * dt;
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        delta = GetMouseDelta();
        _yaw += delta.x * MOUSE_SPEED;
        _pitch -= delta.y * MOUSE_SPEED;
    }
}

void CameraController::clampValues()
{
    if (_distance < MIN_DISTANCE)
        _distance = MIN_DISTANCE;
    if (_distance > MAX_DISTANCE)
        _distance = MAX_DISTANCE;
    if (_pitch < MIN_PITCH)
        _pitch = MIN_PITCH;
    if (_pitch > MAX_PITCH)
        _pitch = MAX_PITCH;
}

void CameraController::update()
{
    float dt = GetFrameTime();

    handleZoom(dt);
    handleRotation(dt);
    clampValues();
    applySpherical();
}
