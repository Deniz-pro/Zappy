/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** Config.hpp
*/

#ifndef CONFIG_HPP_
    #define CONFIG_HPP_

inline constexpr float TILE_SPACING = 2.0f;
inline constexpr float TILE_SIZE = 1.8f;
inline constexpr float TILE_HEIGHT = 0.2f;

inline constexpr float ZOOM_SPEED = 1.5f;
inline constexpr float ROTATE_SPEED = 0.03f;
inline constexpr float MOUSE_SPEED = 0.005f;
inline constexpr float MIN_DISTANCE = 2.0f;
inline constexpr float MAX_DISTANCE = 200.0f;
inline constexpr float MIN_PITCH = 0.1f;
inline constexpr float MAX_PITCH = 1.5f;

inline constexpr int HUD_FONT_SIZE = 14;
inline constexpr int HUD_BOX_SIZE = 14;
inline constexpr int HUD_LEGEND_OFFSET = 160;
inline constexpr int RESOURCE_FONT_SIZE = 10;

//inline constexpr const char *MODEL_PATH = "assets/trantorian.glb";
inline constexpr const char *MODEL_PATH = "assets/impostor.glb";
inline constexpr float PLAYER_HEIGHT = 1.0f;
inline constexpr float LABEL_Y_OFFSET = 0.9f;
inline constexpr float SELECTION_RADIUS_SQ = 400.0f;

inline constexpr float FLASH_DURATION = 0.6f;
inline constexpr float HATCH_DURATION = 0.5f;
inline constexpr float DIE_DURATION = 0.4f;
inline constexpr float BROADCAST_DURATION = 2.0f;
inline constexpr int BROADCAST_FONT_SIZE = 8;
inline constexpr int BROADCAST_LINE_HEIGHT = 12;
inline constexpr int BROADCAST_MAX_MESSAGES = 1;

inline constexpr int FREQ_BTN_X = 10;
inline constexpr int FREQ_BTN_Y = 85;
inline constexpr int FREQ_BTN_W = 28;
inline constexpr int FREQ_BTN_H = 18;
inline constexpr int FREQ_BTN_GAP = 4;
inline constexpr int FREQ_TEAM_Y = 112;
inline constexpr int FREQ_MAX = 1000;
inline constexpr int FREQ_DEFAULT = 100;

inline constexpr float SKYBOX_SCALE = 500.0f;

#endif
