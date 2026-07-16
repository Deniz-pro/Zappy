/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** HudClicks.cpp
*/

#include "Window.hpp"
#include "gui/renderer/Config.hpp"
#include <raylib.h>
#include <vector>
#include <algorithm>
#include <string>

int Window::getTeamListStartY() const
{
    int y = _state.getMapWidth() > 0 ? FREQ_TEAM_Y : 70;

    return y + HUD_FONT_SIZE + 4;
}

std::vector<int> Window::getSortedTeamPlayerIds(const std::string &team) const
{
    std::vector<int> ids;

    for (const auto &[id, p] : _state.getPlayers())
        if (p.team == team)
            ids.push_back(id);
    std::sort(ids.begin(), ids.end());
    return ids;
}

bool Window::handleTeamClick(const std::string &team, Vector2 mouse, int x, int &y)
{
    Rectangle rect = {(float)x, (float)y, 210.0f, (float)HUD_BOX_SIZE};

    if (!CheckCollisionPointRec(mouse, rect))
        return false;
    _state.selectedTeam = (_state.selectedTeam == team) ? "" : team;
    return true;
}

void Window::selectPlayer(int id)
{
    _state.selectedPlayerId = (_state.selectedPlayerId == id) ? -1 : id;
    if (_state.selectedPlayerId != -1)
        _client.sendMessage("pin #" + std::to_string(id) + "\n");
}

bool Window::handlePlayerListClick(const std::string &team, Vector2 mouse, int x, int &y)
{
    for (int id : getSortedTeamPlayerIds(team)) {
        Rectangle rect = {(float)x, (float)y, 210.0f, (float)(HUD_FONT_SIZE + 2)};
        if (CheckCollisionPointRec(mouse, rect)) {
            selectPlayer(id);
            return true;
        }
        y += HUD_FONT_SIZE + 2;
    }
    return false;
}

void Window::handleHudClicks()
{
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        return;
    Vector2 mouse = GetMousePosition();
    int x = 10;
    int y = getTeamListStartY();
    for (const auto &team : _state.getTeams()) {
        if (handleTeamClick(team, mouse, x, y))
            return;
        y += HUD_BOX_SIZE + 4;
        if (_state.selectedTeam == team && handlePlayerListClick(team, mouse, x, y))
            return;
    }
}
