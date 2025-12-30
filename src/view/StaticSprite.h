#pragma once
#include <raylib.h>

struct StaticSprite
{
    Texture2D texture;
    Vector2 position;
    Rectangle source_rectangle;
    Rectangle dest_rectangle;
};
