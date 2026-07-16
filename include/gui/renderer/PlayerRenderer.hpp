/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** PlayerRenderer.hpp
*/

#ifndef PLAYERRENDERER_HPP_
    #define PLAYERRENDERER_HPP_
    #include "state/GameState.hpp"
    #include "gui/renderer/Config.hpp"
    #include "gui/renderer/PlayerRendererTypes.hpp"
    #include <string>

class PlayerRenderer {
    public:
        PlayerRenderer() = default;
        ~PlayerRenderer();
        void ensureLoaded();
        void draw(const GameState &state) const;
        void drawLabels(const GameState &state,
            const Camera3D &camera) const;
        void handleSelection(GameState &state,
            const Camera3D &camera) const;
        static Color getTeamColor(const std::string &team);
    private:
        static Vector3 playerWorldPos(const Player &player,
            float heightAboveTile = 0.0f);
        DrawContext buildContext(int id, const Player &p,
            int selectedId) const;
        void drawPlayer(const DrawContext &ctx) const;
        PlayerModel loadPlayerAsset() const;
        void applyModelScale(PlayerModel &playerModel) const;
        void drawModel(const PlayerModel &pm, Vector3 base,
            Orientation o) const;
        void drawTeamRing(Vector3 base, Color color) const;
        int findClickedPlayer(const GameState &state,
            const Camera3D &camera, Vector2 mouse) const;
        void unload();
        PlayerModel _model;
        bool _loaded = false;
};

#endif
