/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Window.hpp
*/

#ifndef WINDOW_HPP_
    #define WINDOW_HPP_
    #include "gui/network/Client.hpp"
    #include <vector>
    #include "gui/renderer/Config.hpp"
    #include "gui/MusicManager.hpp"
    #include <raylib.h>
    #include "state/GameState.hpp"
    #include "gui/parser/Parser.hpp"
    #include "gui/renderer/MapRenderer.hpp"
    #include "gui/renderer/HudRenderer.hpp"
    #include "gui/renderer/MinimapRenderer.hpp"

struct WindowConfig {
    int width = 1920;
    int height = 1080;
    const char *title = "ZAPPY";
};

class Window {
    public:
        Window(const WindowConfig &config, Client &client);
        ~Window();
        void run();
    private:
        void update();
        void draw();
        void processNetwork();
        void handleFreqButtons();
        void handleHudClicks();
        void handleInputs();
        bool handleTeamClick(const std::string &team, Vector2 mouse, int x, int &y);
        bool handlePlayerListClick(const std::string &team, Vector2 mouse, int x, int &y);
        void selectPlayer(int id);
        std::vector<int> getSortedTeamPlayerIds(const std::string &team) const;
        int getTeamListStartY() const;
        int getClickedFreqButton(Vector2 mouse) const;
        int getFreqBase() const;
        int computeNewFreq(int idx, int base) const;
        WindowConfig _config;
        Client &_client;
        GameState _state;
        Parser _parser;
        MapRenderer _mapRenderer;
        HudRenderer _hudRenderer;
        MinimapRenderer _minimapRenderer;
        MusicManager _music;
};

#endif
