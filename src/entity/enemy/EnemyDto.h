#pragma once;
#include "raylib.h"
#include "../common/Direction.h"
#include "EnemyState.h"
#include <cstdint>

struct EnemyDto
{
    uint32_t id;
    Vector2 position;
    Direction direction;
    EnemyState state;
};
