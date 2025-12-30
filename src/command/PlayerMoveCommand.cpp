#include "PlayerMoveCommand.h"
#include "../entity/player/Player.h"

void PlayerMoveCommand::Execute(uint32_t player_id)
{
    auto option = entity_manager->GetEntity(player_id);

    if (!option.has_value())
        return;

    auto player = std::static_pointer_cast<Player>(option.value());

    Vector2 moveDir = {0, 0};

    if (IsKeyDown(KEY_UP))
    {
        moveDir.x += -1;
        moveDir.y += -1;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        moveDir.x += 1;
        moveDir.y += 1;
    }
    if (IsKeyDown(KEY_LEFT))
    {
        moveDir.x += -1;
        moveDir.y += 1;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        moveDir.x += 1;
        moveDir.y += -1;
    }

    player->Move(moveDir, {});
}