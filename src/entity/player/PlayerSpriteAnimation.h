#pragma once
#include "raylib.h"
#include "../../common/ResourceManager.h"
#include "PlayerDirection.h"
#include "PlayerState.h"

class PlayerSpriteAnimation
{
private:
    int currentFrame = 0;
    float runningTime = 0.0f;
    Rectangle sourceRectangle;
    Texture2D currentTexture;

public:
    PlayerSpriteAnimation() = default;
    ~PlayerSpriteAnimation() = default;

    void Animate(PlayerDirection playerDirection, PlayerState state);

    Rectangle GetSourceRectangle();
    Texture2D GetCurrentTexture();
    void Reset();
};