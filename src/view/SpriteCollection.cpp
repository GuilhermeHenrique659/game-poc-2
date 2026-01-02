#include "SpriteCollection.h"

void SpriteCollection::AddSprite(Direction direction, SpriteSheet sprite_key)
{
    sprites[direction] = sprite_key;
}

SpriteSheet SpriteCollection::GetSprite(Direction direction)
{
    return sprites[direction];
}
