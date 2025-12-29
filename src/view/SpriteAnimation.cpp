#include "SpriteAnimation.h"
#include "../common/ResourceManager.h"

Rectangle SpriteAnimation::GetSourceRectangle()
{
    return source_rectangle;
}

Texture2D SpriteAnimation::GetTexture()
{
    return texture;
}

void SpriteAnimation::Reset()
{
    current_frame = 0;
    elapsed_time = 0.0f;
}

void SpriteAnimation::Animate(SpriteCollection &sprite_collection, Direction direction)
{
    auto sprite_sheet = sprite_collection.GetSprite(direction);

    auto sprite_texture = ResourceManager::Get().GetTexture(sprite_sheet.texture_key);

    const auto columns = sprite_sheet.columns;
    const auto rows = sprite_sheet.rows;

    elapsed_time += GetFrameTime();
    if (elapsed_time >= frame_time)
    {
        elapsed_time = 0.0f;

        current_frame++;
        if (current_frame >= sprite_sheet.total_frames)
        {
            current_frame = 0;
        }
    }

    auto columnFrame = current_frame % columns;
    auto rowFrame = current_frame / columns;

    source_rectangle = {
        (float)sprite_sheet.frame_width * columnFrame,
        (float)sprite_sheet.frame_height * rowFrame,
        (float)sprite_sheet.frame_width,
        (float)sprite_sheet.frame_height};

    texture = sprite_texture;
}