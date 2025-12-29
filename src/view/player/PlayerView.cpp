#include "PlayerView.h"
#include "../../entity/player/Player.h"

static SpriteCollection CreateIdleCollection()
{
    SpriteCollection idle_collection = SpriteCollection();

    idle_collection.AddSprite(Direction::DOWN, SpriteSheet{"idle_stand_down", 320, 320, 16, 4, 4});
    idle_collection.AddSprite(Direction::RIGHT_DOWN, SpriteSheet{"idle_stand_down_right", 320, 320, 16, 4, 4});
    idle_collection.AddSprite(Direction::RIGHT, SpriteSheet{"idle_stand_right", 320, 320, 16, 4, 4});
    idle_collection.AddSprite(Direction::RIGHT_UP, SpriteSheet{"idle_stand_up_right", 320, 320, 16, 4, 4});
    idle_collection.AddSprite(Direction::UP, SpriteSheet{"idle_stand_up", 320, 320, 16, 4, 4});
    idle_collection.AddSprite(Direction::LEFT_UP, SpriteSheet{"idle_stand_up_left", 320, 320, 16, 4, 4});
    idle_collection.AddSprite(Direction::LEFT, SpriteSheet{"idle_stand_left", 320, 320, 16, 4, 4});
    idle_collection.AddSprite(Direction::LEFT_DOWN, SpriteSheet{"idle_stand_down_left", 320, 320, 16, 4, 4});

    return idle_collection;
}

static SpriteCollection CreateRunningCollection()
{
    SpriteCollection running_collection = SpriteCollection();

    running_collection.AddSprite(Direction::DOWN, SpriteSheet{"run_down", 320, 320, 16, 4, 4});
    running_collection.AddSprite(Direction::RIGHT_DOWN, SpriteSheet{"run_down_right", 320, 320, 16, 4, 4});
    running_collection.AddSprite(Direction::RIGHT, SpriteSheet{"run_right", 320, 320, 16, 4, 4});
    running_collection.AddSprite(Direction::RIGHT_UP, SpriteSheet{"run_up_right", 320, 320, 16, 4, 4});
    running_collection.AddSprite(Direction::UP, SpriteSheet{"run_up", 320, 320, 16, 4, 4});
    running_collection.AddSprite(Direction::LEFT_UP, SpriteSheet{"run_up_left", 320, 320, 16, 4, 4});
    running_collection.AddSprite(Direction::LEFT, SpriteSheet{"run_left", 320, 320, 16, 4, 4});
    running_collection.AddSprite(Direction::LEFT_DOWN, SpriteSheet{"run_down_left", 320, 320, 16, 4, 4});

    return running_collection;
}

const std::unordered_map<PlayerState, SpriteCollection> PlayerView::player_collection = {
    {PlayerState::Idle, CreateIdleCollection()},
    {PlayerState::Running, CreateRunningCollection()},
    // Additional states like Running and Attacking can be added here
};

void PlayerView::Draw(std::shared_ptr<Entity> entity, std::shared_ptr<SpriteAnimation> animation)
{
    auto player = std::dynamic_pointer_cast<Player>(entity);
    if (!player)
        return;

    auto collection = player_collection.at(player->GetState());

    animation->Animate(collection, entity->GetEntityDirection());
}