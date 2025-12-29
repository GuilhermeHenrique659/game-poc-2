#include "PlayerView.h"

SpriteCollection &GetPlayerSpriteCollection()
{
    static SpriteCollection sprite_collection;
    sprite_collection.AddSprite(Direction::DOWN, {.texture_key = "idle_stand_down",
                                                  .frame_width = 320,
                                                  .frame_height = 320,
                                                  .total_frames = 16,
                                                  .columns = 4,
                                                  .rows = 4});

    return sprite_collection;
}