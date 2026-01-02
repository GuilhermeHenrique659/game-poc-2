#pragma once

#include <memory>
#include <unordered_map>

#include "../../entity/common/Entity.h"
#include "../SpriteCollection.h"
#include "../SpriteAnimation.h"
#include "../../entity/player/PlayerState.h"

class PlayerView
{
private:
    static const std::unordered_map<PlayerState, SpriteCollection> player_collection;

public:
    PlayerView() = default;
    ~PlayerView() = default;

    static void Draw(std::shared_ptr<Entity> entity, std::shared_ptr<SpriteAnimation> animation);
};