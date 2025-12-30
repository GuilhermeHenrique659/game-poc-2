#pragma once
#include <vector>
#include <tmxlite/Map.hpp>
#include <unordered_map>
#include "../../view/StaticSprite.h"
#include "../../common/CollisionLines.h"

class GameMap
{
private:
    tmx::Map map;
    std::unordered_map<std::string, std::vector<StaticSprite>> map_sprites;
    std::vector<CollisionLines> collisition_tiles_lines;

public:
    GameMap() = default;
    ~GameMap() = default;

    std::vector<CollisionLines> GetCollisionLines();
    void Init();
    void Draw(const std::string &layer_name);
};
