#pragma once
#include <raylib.h>
#include "SpriteCollection.h"

class SpriteAnimation
{
private:
    Texture2D texture;
    Rectangle source_rectangle;
    int current_frame;
    const float frame_time;
    float elapsed_time;

public:
    SpriteAnimation() : current_frame(0), frame_time(0.08f), elapsed_time(0.0f) {};
    ~SpriteAnimation() = default;

    void Animate(SpriteCollection &sprite_collection, Direction direction);

    Rectangle GetSourceRectangle();
    Texture2D GetTexture();
    void Reset();
};