/*
** EPITECH PROJECT, 2026
** G-YEP-400-PAR-4-1-zappy-2
** File description:
** Egg.hpp
*/

#ifndef EGG_HPP
    #define EGG_HPP
    #include <string>

namespace zappy {
    enum class EggAnim {
        IDLE,
        HATCHING,
        DYING
    };

    struct Egg {
        int id = 0;
        int x = 0;
        int y = 0;
        std::string team;
        EggAnim anim = EggAnim::IDLE;
        float animTimer = 0.0f;
    };
}

#endif
