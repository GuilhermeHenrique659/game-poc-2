#pragma once;

#include <string>
#include <unordered_map>
#include "../entity/common/Direction.h"
#include <raylib.h>
#include <math.h>
#include "../config.h"
#include "../common/ResourceManager.h"

struct SpriteSheet
{
    int totalFrames;
    int columns;
    int rows;
    float frameWidth;
    float frameHeight;
    std::string textureName;
};

class EntitySpriteCollection
{
private:
    std::unordered_map<Direction, SpriteSheet> collection;

public:
    EntitySpriteCollection() = default;
    ~EntitySpriteCollection() = default;

    SpriteSheet getByDirection(Direction direction)
    {
        return collection[direction];
    }
    void registerSprite(Direction direction, SpriteSheet sprite)
    {
        collection[direction] = sprite;
    }
};

class EntityAnimationSprite
{
private:
    Rectangle sourceRectangle;
    Texture2D currentTexture;
    int currentFrame;
    float runningTime;
    const float frameTime;

public:
    EntityAnimationSprite() : currentFrame(0), runningTime(0.0f), frameTime(0.08f) {}
    ~EntityAnimationSprite() = default;

    Rectangle GetSourceRectangle()
    {
        return sourceRectangle;
    }

    Texture2D GetCurrentTexture()
    {
        return currentTexture;
    }

    void Reset()
    {
        currentFrame = 0;
        runningTime = 0.0f;
    }

    void Animate(Direction dir, EntitySpriteCollection collection)
    {
        SpriteSheet spriteSheet = collection.getByDirection(dir);

        currentTexture = ResourceManager::Get().GetTexture(spriteSheet.textureName);

        const int columns = spriteSheet.columns;
        const int rows = spriteSheet.rows;

        runningTime += GetFrameTime();
        if (runningTime >= frameTime)
        {
            runningTime = 0.0f;

            currentFrame++;
            if (currentFrame >= spriteSheet.totalFrames)
            {
                currentFrame = 0;
            }
        }

        int columnFrame = currentFrame % columns;
        int rowFrame = currentFrame / columns;

        sourceRectangle = {
            spriteSheet.frameWidth * columnFrame,
            spriteSheet.frameHeight * rowFrame,
            spriteSheet.frameWidth,
            spriteSheet.frameHeight};
    }
};