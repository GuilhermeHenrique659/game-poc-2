#include "PlayerView.h"
#include "../../entity/player/Player.h"

static SpriteCollection CreateIdleCollection()
{
    int frame_count = 16;
    int columns = 4;
    int rows = 4;
    float frame_time = 0.06f;

    SpriteCollection idle_collection = SpriteCollection();

    idle_collection.AddSprite(Direction::DOWN, SpriteSheet{"idle_stand_down", 320, 320, frame_count, columns, rows, frame_time});
    idle_collection.AddSprite(Direction::RIGHT_DOWN, SpriteSheet{"idle_stand_down_right", 320, 320, frame_count, columns, rows, frame_time});
    idle_collection.AddSprite(Direction::RIGHT, SpriteSheet{"idle_stand_right", 320, 320, frame_count, columns, rows, frame_time});
    idle_collection.AddSprite(Direction::RIGHT_UP, SpriteSheet{"idle_stand_up_right", 320, 320, frame_count, columns, rows, frame_time});
    idle_collection.AddSprite(Direction::UP, SpriteSheet{"idle_stand_up", 320, 320, frame_count, columns, rows, frame_time});
    idle_collection.AddSprite(Direction::LEFT_UP, SpriteSheet{"idle_stand_up_left", 320, 320, frame_count, columns, rows, frame_time});
    idle_collection.AddSprite(Direction::LEFT, SpriteSheet{"idle_stand_left", 320, 320, frame_count, columns, rows, frame_time});
    idle_collection.AddSprite(Direction::LEFT_DOWN, SpriteSheet{"idle_stand_down_left", 320, 320, frame_count, columns, rows, frame_time});

    return idle_collection;
}

static SpriteCollection CreateRunningCollection()
{
    int frame_count = 15;
    int columns = 5;
    int rows = 3;
    float frame_time = 0.06f;

    SpriteCollection running_collection = SpriteCollection();

    running_collection.AddSprite(Direction::DOWN, SpriteSheet{"run_down", 320, 320, frame_count, columns, rows, frame_time});
    running_collection.AddSprite(Direction::RIGHT_DOWN, SpriteSheet{"run_down_right", 320, 320, frame_count, columns, rows, frame_time});
    running_collection.AddSprite(Direction::RIGHT, SpriteSheet{"run_right", 320, 320, frame_count, columns, rows, frame_time});
    running_collection.AddSprite(Direction::RIGHT_UP, SpriteSheet{"run_up_right", 320, 320, frame_count, columns, rows, frame_time});
    running_collection.AddSprite(Direction::UP, SpriteSheet{"run_up", 320, 320, frame_count, columns, rows, frame_time});
    running_collection.AddSprite(Direction::LEFT_UP, SpriteSheet{"run_up_left", 320, 320, frame_count, columns, rows, frame_time});
    running_collection.AddSprite(Direction::LEFT, SpriteSheet{"run_left", 320, 320, frame_count, columns, rows, frame_time});
    running_collection.AddSprite(Direction::LEFT_DOWN, SpriteSheet{"run_down_left", 320, 320, frame_count, columns, rows, frame_time});

    return running_collection;
}

static SpriteCollection CreateAttackingCollection()
{
    int frame_count = 16;
    int columns = 4;
    int rows = 4;
    float frame_time = 0.08f;

    SpriteCollection attacking_collection = SpriteCollection();

    attacking_collection.AddSprite(Direction::DOWN, SpriteSheet{"attack_down", 320, 320, frame_count, columns, rows, frame_time});
    attacking_collection.AddSprite(Direction::RIGHT_DOWN, SpriteSheet{"attack_down_right", 320, 320, frame_count, columns, rows, frame_time});
    attacking_collection.AddSprite(Direction::RIGHT, SpriteSheet{"attack_right", 320, 320, frame_count, columns, rows, frame_time});
    attacking_collection.AddSprite(Direction::RIGHT_UP, SpriteSheet{"attack_up_right", 320, 320, frame_count, columns, rows, frame_time});
    attacking_collection.AddSprite(Direction::UP, SpriteSheet{"attack_up", 320, 320, frame_count, columns, rows, frame_time});
    attacking_collection.AddSprite(Direction::LEFT_UP, SpriteSheet{"attack_up_left", 320, 320, frame_count, columns, rows, frame_time});
    attacking_collection.AddSprite(Direction::LEFT, SpriteSheet{"attack_left", 320, 320, frame_count, columns, rows, frame_time});
    attacking_collection.AddSprite(Direction::LEFT_DOWN, SpriteSheet{"attack_down_left", 320, 320, frame_count, columns, rows, frame_time});

    return attacking_collection;
}

static SpriteCollection CreateAttacking2Collection()
{
    int frame_count = 16;
    int columns = 4;
    int rows = 4;
    float frame_time = 0.08f;

    SpriteCollection attacking_collection = SpriteCollection();

    attacking_collection.AddSprite(Direction::DOWN, SpriteSheet{"attack_down_2", 320, 320, frame_count, columns, rows, frame_time});
    attacking_collection.AddSprite(Direction::RIGHT_DOWN, SpriteSheet{"attack_down_right_2", 320, 320, frame_count, columns, rows, frame_time});
    attacking_collection.AddSprite(Direction::RIGHT, SpriteSheet{"attack_right_2", 320, 320, frame_count, columns, rows, frame_time});
    attacking_collection.AddSprite(Direction::RIGHT_UP, SpriteSheet{"attack_up_right_2", 320, 320, frame_count, columns, rows, frame_time});
    attacking_collection.AddSprite(Direction::UP, SpriteSheet{"attack_up_2", 320, 320, frame_count, columns, rows, frame_time});
    attacking_collection.AddSprite(Direction::LEFT_UP, SpriteSheet{"attack_up_left_2", 320, 320, frame_count, columns, rows, frame_time});
    attacking_collection.AddSprite(Direction::LEFT, SpriteSheet{"attack_left_2", 320, 320, frame_count, columns, rows, frame_time});
    attacking_collection.AddSprite(Direction::LEFT_DOWN, SpriteSheet{"attack_down_left_2", 320, 320, frame_count, columns, rows, frame_time});

    return attacking_collection;
}

const std::unordered_map<PlayerState, SpriteCollection> PlayerView::player_collection = {
    {PlayerState::Idle, CreateIdleCollection()},
    {PlayerState::Running, CreateRunningCollection()},
    {PlayerState::Attacking, CreateAttackingCollection()},
};

void PlayerView::Draw(std::shared_ptr<Entity> entity, std::shared_ptr<SpriteAnimation> animation)
{
    auto player = std::dynamic_pointer_cast<Player>(entity);
    if (!player)
        return;

    auto collection = player_collection.at(player->GetState());

    animation->Animate(collection, entity->GetEntityDirection());
}