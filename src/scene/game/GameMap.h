#pragma once
#include <vector>
#include <tmxlite/Map.hpp>
#include <unordered_map>
#include "../../view/StaticSprite.h"

class GameMap
{
private:
    tmx::Map map;
    std::unordered_map<std::string, std::vector<StaticSprite>> map_sprites;
    std::vector<Rectangle> collisition_tiles;

public:
    GameMap() = default;
    ~GameMap() = default;

    std::vector<Rectangle> GetCollisionTiles();
    void Init();
    void Draw(const std::string &layer_name);
};
