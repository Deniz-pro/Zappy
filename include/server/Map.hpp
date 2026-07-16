/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Map.hpp
*/

#ifndef MAP_HPP
    #define MAP_HPP
    #include <vector>

namespace zappy {
    struct Tile {
        int food = 0;
        int linemate = 0;
        int deraumere = 0;
        int sibur = 0;
        int mendiane = 0;
        int phiras = 0;
        int thystame = 0;
    };

    class Map {
        public:
            Map(int width, int height);
            void buildMap();
            int getWidth() const;
            int getHeight() const;
            bool isInBounds(int x, int y) const;
            int wrapX(int x) const;
            int wrapY(int y) const;
            Tile &getTile(int x, int y);
            const Tile &getTile(int x, int y) const;
            const std::vector<std::vector<Tile>> &getTiles() const;
        private:
            int _width;
            int _height;
            std::vector<std::vector<Tile>> _tiles;
    };
}

#endif
