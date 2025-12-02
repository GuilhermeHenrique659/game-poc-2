#pragma once
#include "raylib.h"
#include "../../common/ResourceManager.h"
#include "EnemyDirection.h"
#include "EnemyState.h"

class EnemySpriteAnimation
{
private:
    int currentFrame = 0;
    float runningTime = 0.0f;
    Rectangle sourceRectangle;
    Texture2D currentTexture;

public:
    EnemySpriteAnimation() = default;
    ~EnemySpriteAnimation() = default;

    void Animate(EnemyDirection playerDirection, EnemyState state);

    Rectangle GetSourceRectangle();
    Texture2D GetCurrentTexture();
    void Reset();
};