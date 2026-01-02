#include "PlayerAttackCommand.h"
#include "../entity/player/Player.h"

void PlayerAttackCommand::Execute(uint32_t player_id, const Inputs &input)
{
    auto player_entity = entity_manager->GetEntity(player_id);

    if (!player_entity.has_value())
    {
        return;
    }

    auto player = std::dynamic_pointer_cast<Player>(player_entity.value());
    if (!player)
    {
        return;
    }

    if (input.attack || player->GetState() == PlayerState::Attacking)
        player->Attack();
}