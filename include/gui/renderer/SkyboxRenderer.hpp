/*
** EPITECH PROJECT, 2026
** ZAPPY
** File description:
** SkyboxRenderer.hpp
*/

#ifndef SKYBOXRENDERER_HPP_
    #define SKYBOXRENDERER_HPP_
    #include <raylib.h>

class SkyboxRenderer {
    public:
        SkyboxRenderer() = default;
        ~SkyboxRenderer();
        SkyboxRenderer(const SkyboxRenderer &) = delete;
        SkyboxRenderer &operator=(const SkyboxRenderer &) = delete;
        SkyboxRenderer(SkyboxRenderer &&) = delete;
        SkyboxRenderer &operator=(SkyboxRenderer &&) = delete;
        void load();
        void draw(const Camera3D &camera) const;
        void unload();
    private:
        bool loadTexture();
        static Image generateStarfieldImage(int size);
        Model _cube{};
        Texture2D _texture{};
        bool _loaded = false;
};

#endif
