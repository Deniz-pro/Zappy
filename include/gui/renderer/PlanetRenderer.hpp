/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** PlanetRenderer.hpp
*/

#ifndef PLANETRENDERER_HPP_
    #define PLANETRENDERER_HPP_
    #include <raylib.h>

class PlanetRenderer {
    public:
        ~PlanetRenderer();
        void draw(Vector3 center, float radius) const;
        void drawOrbitalRing(Vector3 center, float radius) const;
        void drawShootingStars(Vector3 center, float radius) const;
    private:
        mutable Model _model  = {};
        mutable Texture2D _tex = {};
        mutable bool _loaded = false;
};

#endif
