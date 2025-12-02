#include "EnemySpriteAnimation.h"
#include "../../config.h"
#include "math.h"

struct SpriteSheetInfo
{
    int totalFrames;
    int columns;
    int rows;
    float frameWidth;
    float frameHeight;
};

static SpriteSheetInfo CalculateSpriteSheet(Texture2D texture, int totalFrames)
{
    SpriteSheetInfo info;
    info.totalFrames = totalFrames;

    info.columns = (int)sqrtf(totalFrames);
    info.rows = totalFrames / info.columns;

    info.frameWidth = (float)texture.width / info.columns;
    info.frameHeight = (float)texture.height / info.rows;

    return info;
}

static inline float GetFrameWidth(Texture2D texture, float frameCount)
{
    return (float)texture.width / frameCount;
}

Rectangle EnemySpriteAnimation::GetSourceRectangle()
{
    return sourceRectangle;
}

Texture2D EnemySpriteAnimation::GetCurrentTexture()
{
    return currentTexture;
}

void EnemySpriteAnimation::Reset()
{
    currentFrame = 0;
    runningTime = 0.0f;
}

void EnemySpriteAnimation::Animate(EnemyDirection playerDirection, EnemyState state)
{
    int totalFrames;

    switch (state)
    {
    case EnemyState::IDLE:
        totalFrames = 16;
        break;
    case EnemyState::RUN:
        totalFrames = 15;
        break;
    case EnemyState::ATTACK:
    default:
        totalFrames = 16;
        break;
    }

    const float frameTime = 0.08f;

    std::string textureKey = "idle_stand_down";

    switch (playerDirection)
    {
    case EnemyDirection::RIGHT:
        if (state == EnemyState::IDLE)
            textureKey = "idle_stand_right";
        else if (state == EnemyState::ATTACK)
            textureKey = "attack_right";
        else
            textureKey = "run_right";
        break;
    case EnemyDirection::Right_DOWN:
        if (state == EnemyState::IDLE)
            textureKey = "idle_stand_down_right";
        else if (state == EnemyState::ATTACK)
            textureKey = "attack_right_down";
        else
            textureKey = "run_right_down";
        break;
    case EnemyDirection::DOWN:
        if (state == EnemyState::IDLE)
            textureKey = "idle_stand_down";
        else if (state == EnemyState::ATTACK)
            textureKey = "attack_down";
        else
            textureKey = "run_down";
        break;
    case EnemyDirection::LEFT_DOWN:
        if (state == EnemyState::IDLE)
            textureKey = "idle_stand_down_left";
        else if (state == EnemyState::ATTACK)
            textureKey = "attack_left_down";
        else
            textureKey = "run_left_down";
        break;
    case EnemyDirection::LEFT:
        if (state == EnemyState::IDLE)
            textureKey = "idle_stand_left";
        else if (state == EnemyState::ATTACK)
            textureKey = "attack_left";
        else
            textureKey = "run_left";
        break;
    case EnemyDirection::LEFT_UP:
        if (state == EnemyState::IDLE)
            textureKey = "idle_stand_up_left";
        else if (state == EnemyState::ATTACK)
            textureKey = "attack_left_up";
        else
            textureKey = "run_left_up";
        break;
    case EnemyDirection::UP:
        if (state == EnemyState::IDLE)
            textureKey = "idle_stand_up";
        else if (state == EnemyState::ATTACK)
            textureKey = "attack_up";
        else
            textureKey = "run_up";
        break;
    case EnemyDirection::RIGHT_UP:
        if (state == EnemyState::IDLE)
            textureKey = "idle_stand_up_right";
        else if (state == EnemyState::ATTACK)
            textureKey = "attack_right_up";
        else
            textureKey = "run_right_up";
        break;
    default:
        if (state == EnemyState::IDLE)
            textureKey = "idle_stand_down";
        else if (state == EnemyState::ATTACK)
            textureKey = "attack_down";
        else
            textureKey = "run_down";
        break;
    }

    currentTexture = ResourceManager::Get().GetTexture(textureKey);

    SpriteSheetInfo sheet = CalculateSpriteSheet(currentTexture, totalFrames);
    const int columns = sheet.columns;
    const int rows = sheet.rows;

    runningTime += GetFrameTime();
    if (runningTime >= frameTime)
    {
        runningTime = 0.0f;

        currentFrame++;
        if (currentFrame >= totalFrames)
        {
            currentFrame = 0;
        }
    }

    int columnFrame = currentFrame % columns;
    int rowFrame = currentFrame / columns;

    sourceRectangle = {
        SPRITE_FRAME_WIDHT * columnFrame,
        SPRITEH_FRAME_HEIGHT * rowFrame,
        SPRITE_FRAME_WIDHT,
        SPRITEH_FRAME_HEIGHT};
}