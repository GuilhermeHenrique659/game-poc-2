#include "PlayerMoveCommand.h"
#include "../entity/player/Player.h"

void PlayerMoveCommand::Execute(uint32_t player_id)
{
    TraceLog(LOG_INFO, "Executing PlayerMoveCommand for player ID: %d", player_id);
    TraceLog(LOG_INFO, "entity manger address: %p", entity_manager.get());

    auto entities = entity_manager->GetEntity<Player>(player_id);

    /*     auto player = entity_manager->GetEntity<Player>(player_id);

        if (!player.has_value())
            return;

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

        player.value()->Move(moveDir, {}); */
}