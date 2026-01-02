#pragma once
#include "../entity/common/Direction.h"
#include <string>
#include <unordered_map>

struct SpriteSheet
{
    std::string texture_key;
    int frame_width;
    int frame_height;
    int total_frames;
    int columns;
    int rows;
    float frame_time;
};

class SpriteCollection
{
private:
    std::unordered_map<Direction, SpriteSheet> sprites;

public:
    SpriteCollection() = default;
    ~SpriteCollection() = default;

    void AddSprite(Direction direction, SpriteSheet sprite_key);
    SpriteSheet GetSprite(Direction direction);
};