/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** FreqButtons.cpp
*/

#include "Window.hpp"
#include <raylib.h>
#include <algorithm>

int Window::getClickedFreqButton(Vector2 mouse) const
{
    Rectangle rect = {};

    for (int i = 0; i < 4; i++) {
        rect.x = (float)(FREQ_BTN_X + i * (FREQ_BTN_W + FREQ_BTN_GAP));
        rect.y = (float)FREQ_BTN_Y;
        rect.width = (float)FREQ_BTN_W;
        rect.height = (float)FREQ_BTN_H;
        if (CheckCollisionPointRec(mouse, rect))
            return i;
    }
    return -1;
}

int Window::getFreqBase() const
{
    return _state.freq > 0 ? _state.freq : FREQ_DEFAULT;
}

int Window::computeNewFreq(int idx, int base) const
{
    if (idx == 0)
        return std::max(1, base / 10);
    if (idx == 1)
        return std::max(1, base / 2);
    if (idx == 2)
        return std::min(FREQ_MAX, base * 2);
    return std::min(FREQ_MAX, base * 10);
}

void Window::handleFreqButtons()
{
    int idx = 0;
    int newFreq = 0;

    if (_state.getMapWidth() == 0 || !IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return;
    idx = getClickedFreqButton(GetMousePosition());
    if (idx == -1)
        return;
    newFreq = computeNewFreq(idx, getFreqBase());
    _state.freq = newFreq;
    _client.sendMessage("sst " + std::to_string(newFreq) + "\n");
}
