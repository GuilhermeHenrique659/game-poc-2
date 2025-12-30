#include "PlayerMoveCommand.h"
#include "../entity/player/Player.h"

void PlayerMoveCommand::Execute(uint32_t player_id, const Inputs &input)
{
    auto option = entity_manager->GetEntity(player_id);

    if (!option.has_value())
        return;

    auto player = std::dynamic_pointer_cast<Player>(option.value());

    if (player == nullptr)
    {
        TraceLog(LOG_WARNING, "PlayerMoveCommand: Entity is not a Player.");
        return;
    }

    Vector2 moveDir = {0, 0};

    if (input.move_up)
    {
        moveDir.x += -1;
        moveDir.y += -1;
    }
    if (input.move_down)
    {
        moveDir.x += 1;
        moveDir.y += 1;
    }
    if (input.move_left)
    {
        moveDir.x += -1;
        moveDir.y += 1;
    }
    if (input.move_right)
    {
        moveDir.x += 1;
        moveDir.y += -1;
    }

    player->Move(moveDir, {});
}