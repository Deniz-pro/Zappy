/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** MoonRenderer.hpp
*/

#ifndef MOONRENDERER_HPP_
    #define MOONRENDERER_HPP_
    #include <raylib.h>

class MoonRenderer {
    public:
        void draw(Vector3 planetCenter, float planetRadius) const;
};

#endif
