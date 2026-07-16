/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Window.cpp
*/

#include "Window.hpp"
#include <raylib.h>
#include <string>

Window::Window(const WindowConfig &config, Client &client)
    : _config(config), _client(client)
{
    InitWindow(config.width, config.height, config.title);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(0);
    _music.load("assets/kazoorap.mp3");
    _music.setVolume(0.5f);
    _music.play();
}

Window::~Window()
{
    _music.shutdown();
    CloseWindow();
}

void Window::run()
{
    while (!WindowShouldClose() && _client.isConnected()) {
        update();
        draw();
    }
}

void Window::handleInputs()
{
    int prevId = _state.selectedPlayerId;

    handleFreqButtons();
    handleHudClicks();
    _mapRenderer.handleSelection(_state);
    if (_state.selectedPlayerId != -1 && _state.selectedPlayerId != prevId)
        _client.sendMessage("pin #" +
            std::to_string(_state.selectedPlayerId) + "\n");
}

void Window::update()
{
    if (!_client.isConnected())
        return;
    _client.flushSend();
    processNetwork();
    handleInputs();
    _state.tickEffects(GetFrameTime());
    _music.update();
}

void Window::processNetwork()
{
    std::string line;

    while (true) {
        line = _client.receive();
        if (line.empty())
            break;
        _parser.parse(line, _state);
    }
}

void Window::draw()
{
    BeginDrawing();
    ClearBackground(BLACK);
    _mapRenderer.draw(_state);
    _hudRenderer.draw(_state);
    _minimapRenderer.draw(_state);
    EndDrawing();
}
