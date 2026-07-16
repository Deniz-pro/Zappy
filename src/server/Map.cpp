/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Map.cpp
*/

#include "Map.hpp"
#include <stdexcept>

zappy::Map::Map(int width, int height) : _width(width), _height(height)
{
}

int zappy::Map::getWidth() const
{
    return _width;
}

int zappy::Map::getHeight() const
{
    return _height;
}

bool zappy::Map::isInBounds(int x, int y) const
{
    return x >= 0 && x < _width && y >= 0 && y < _height;
}

int zappy::Map::wrapX(int x) const
{
    return ((x % _width) + _width) % _width;
}

int zappy::Map::wrapY(int y) const
{
    return ((y % _height) + _height) % _height;
}

zappy::Tile &zappy::Map::getTile(int x, int y)
{
    if (!isInBounds(x, y))
        throw std::out_of_range("Tile coordinates out of bounds.");
    return _tiles[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)];
}

const zappy::Tile &zappy::Map::getTile(int x, int y) const
{
    if (!isInBounds(x, y))
        throw std::out_of_range("Tile coordinates out of bounds.");
    return _tiles[static_cast<std::size_t>(y)][static_cast<std::size_t>(x)];
}

const std::vector<std::vector<zappy::Tile>> &zappy::Map::getTiles() const
{
    return _tiles;
}

void zappy::Map::buildMap()
{
    if (_width <= 0 || _height <= 0)
        throw std::invalid_argument("Invalid map dimensions.");
    _tiles.clear();
    _tiles.resize(static_cast<std::size_t>(_height));
    for (int y = 0; y < _height; ++y)
        _tiles[static_cast<std::size_t>(y)].resize(static_cast<std::size_t>(_width));
}
