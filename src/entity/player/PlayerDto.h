#pragma once;
#include "raylib.h"
#include "PlayerDirection.h"
#include "PlayerState.h"
#include <cstdint>

struct PlayerDto
{
    uint32_t id;
    Vector2 position;
    PlayerDirection direction;
    PlayerState state;
};
