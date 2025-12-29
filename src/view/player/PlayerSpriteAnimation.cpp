#include "../EntityAnimationSprite.cpp"
#include "../../entity/player/PlayerState.h"
#include "../../entity/common/Direction.h"
#include <vector>

static const std::vector<std::pair<Direction, std::string>> enemyDirectionNames = {
    {Direction::DOWN, "down"},
    {Direction::UP, "up"},
    {Direction::LEFT, "left"},
    {Direction::RIGHT, "right"},
    {Direction::LEFT_UP, "up_left"},
    {Direction::RIGHT_UP, "up_right"},
    {Direction::LEFT_DOWN, "down_left"},
    {Direction::RIGHT_DOWN, "down_right"},
};

static EntitySpriteCollection MakeCollection(
    const std::string &prefix,
    int totalFrames = 16,
    int columns = 4,
    int rows = 4,
    int frameSize = 320)
{
    EntitySpriteCollection collection;

    for (const auto &[direction, name] : enemyDirectionNames)
    {
        SpriteSheet sheet{
            .totalFrames = totalFrames,
            .columns = columns,
            .rows = rows,
            .frameWidth = frameSize,
            .frameHeight = frameSize,
            .textureName = prefix + "_" + name};

        collection.registerSprite(direction, sheet);
    }

    return collection;
}

static EntitySpriteCollection GetByState(PlayerState state)
{
    switch (state)
    {
    case PlayerState::IDLE:
        return MakeCollection("idle_stand");

    case PlayerState::RUN:
        return MakeCollection("run");

    case PlayerState::ATTACK:
        return MakeCollection("attack");

    default:
        return EntitySpriteCollection();
    }
}
