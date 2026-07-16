/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** EggUtils.hpp
*/

#ifndef EGGUTILS_HPP_
    #define EGGUTILS_HPP_
    #include "state/Egg.hpp"
    #include <raylib.h>

struct EggVisual {
    Color color = WHITE;
    float scale = 1.0f;
    float alpha = 1.0f;
};

class EggUtils {
    public:
        static EggVisual computeVisual(const zappy::Egg &egg);
};

#endif
