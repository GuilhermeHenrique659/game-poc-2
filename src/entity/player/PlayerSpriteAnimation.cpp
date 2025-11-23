#include "PlayerSpriteAnimation.h"

float GetFrameWidth(Texture2D texture, float frameCount)
{
    return (float)texture.width / frameCount;
}

Rectangle PlayerSpriteAnimation::GetSourceRectangle()
{
    return sourceRectangle;
}

Texture2D PlayerSpriteAnimation::GetCurrentTexture()
{
    return currentTexture;
}

void PlayerSpriteAnimation::Animate(PlayerDirection playerDirection, bool isIdle)
{
    const float runningFrameCount = 10.0f;
    const float idleFrameCount = 8.0f;

    if (isIdle)
    {
        currentTexture = ResourceManager::Get().GetTexture("idle_sheet");
        sourceRectangle = {GetFrameWidth(currentTexture, idleFrameCount) * playerDirection, 0.0f, GetFrameWidth(currentTexture, idleFrameCount), (float)currentTexture.height};
        return;
    }

    switch (playerDirection)
    {
    case PlayerDirection::RIGHT:
        currentTexture = ResourceManager::Get().GetTexture("run_right");
        break;
    case PlayerDirection::Right_DOWN:
        currentTexture = ResourceManager::Get().GetTexture("run_right_down");
        break;
    case PlayerDirection::DOWN:
        currentTexture = ResourceManager::Get().GetTexture("run_down");
        break;
    case PlayerDirection::LEFT_DOWN:
        currentTexture = ResourceManager::Get().GetTexture("run_left_down");
        break;
    case PlayerDirection::LEFT:
        currentTexture = ResourceManager::Get().GetTexture("run_left");
        break;
    case PlayerDirection::LEFT_UP:
        currentTexture = ResourceManager::Get().GetTexture("run_left_up");
        break;
    case PlayerDirection::UP:
        currentTexture = ResourceManager::Get().GetTexture("run_up");
        break;
    case PlayerDirection::RIGHT_UP:
        currentTexture = ResourceManager::Get().GetTexture("run_right_up");
        break;
    default:
        currentTexture = ResourceManager::Get().GetTexture("run_down");
        break;
    }

    runningTime += GetFrameTime();
    if (runningTime >= 0.08f) // velocidade da animação (8 frames = ~0.64s total)
    {

        runningTime = 0.0f;
        currentFrame = (currentFrame + 1) % (int)runningFrameCount;
    }

    sourceRectangle = {GetFrameWidth(currentTexture, runningFrameCount) * currentFrame, 0.0f, GetFrameWidth(currentTexture, runningFrameCount), (float)currentTexture.height};
}