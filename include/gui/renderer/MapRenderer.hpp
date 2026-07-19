/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** MapRenderer.hpp
*/

#ifndef MAPRENDERER_HPP_
    #define MAPRENDERER_HPP_
    #include "state/GameState.hpp"
    #include "gui/renderer/CameraController.hpp"
    #include "gui/renderer/PlayerRenderer.hpp"
    #include "gui/renderer/EggUtils.hpp"
    #include "gui/renderer/Config.hpp"
    #include "gui/renderer/SkyboxRenderer.hpp"
    #include "gui/renderer/PlanetRenderer.hpp"
    #include "gui/renderer/MoonRenderer.hpp"
    #include <deque>
    #include <memory>
    #include <vector>

inline constexpr Vector2 RESOURCE_OFFSETS[7] = {
    {-0.35f, -0.35f}, { 0.0f, -0.35f}, { 0.35f, -0.35f},
    {-0.35f,  0.0f }, { 0.35f,  0.0f },
    {-0.35f,  0.35f}, { 0.0f,  0.35f}
};

class MapRenderer {
    public:
        MapRenderer() = default;
        ~MapRenderer();
        void draw(GameState &state);
        void handleSelection(GameState &state);
    private:
        void initCamera(int mapW, int mapH);
        void drawScene(GameState &state);
        void drawTiles(const GameState &state) const;
        void drawTile(int col, int row, const Tile &tile) const;
        void drawResourceMeshes(const GameState &state) const;
        void drawResourceMesh(const Tile &tile, int col, int row) const;
        void drawResourceShape(int type, Vector3 base, Color color, int count) const;
        void drawEggs(const GameState &state) const;
        void drawEggMesh(int x, int y, const EggVisual &visual) const;
        void drawIncantations(const GameState &state) const;
        void drawExpulsions(const GameState &state) const;
        void drawBroadcasts(const GameState &state) const;
        void drawBroadcastStack(int playerId,
            const std::deque<BroadcastMessage> &msgs, Vector2 screen) const;
        void drawPlayerBroadcasts(int playerId,
            const std::deque<BroadcastMessage> &deque, Vector2 screen) const;
        void drawBroadcastBubble(const std::string &label, int centerX,
            int y, float alpha) const;
        std::unique_ptr<CameraController> _camera;
        PlayerRenderer _playerRenderer;
        SkyboxRenderer _skybox;
        PlanetRenderer _planet;
        MoonRenderer _moon;
};

#endif
