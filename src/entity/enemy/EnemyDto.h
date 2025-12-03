#pragma once;
#include "raylib.h"
#include "EnemyDirection.h"
#include "EnemyState.h"
#include <cstdint>

struct EnemyDto
{
    uint32_t id;
    Vector2 position;
    EnemyDirection direction;
    EnemyState state;
};
