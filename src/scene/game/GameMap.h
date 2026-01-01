#pragma once
#include <vector>
#include <tmxlite/Map.hpp>
#include <unordered_map>
#include "../../view/Sprite.h"
#include "../../common/CollisionLines.h"

class GameMap
{
private:
    tmx::Map map;
    std::unordered_map<std::string, std::vector<Sprite>> map_sprites;
    std::vector<CollisionLines> collisition_tiles_lines;

public:
    GameMap() = default;
    ~GameMap() = default;

    std::vector<CollisionLines> GetCollisionLines();
    void Init();
    void Draw(const std::string &layer_name);
    std::vector<Sprite> &GetSpritesByLayer(const std::string &layer_name);
};
