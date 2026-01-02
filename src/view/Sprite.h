#pragma once
#include <raylib.h>
#include <string>

struct Sprite
{
    Texture2D texture;
    Vector2 position;
    Rectangle source_rectangle;
    Rectangle dest_rectangle;
    const char *type;
};
