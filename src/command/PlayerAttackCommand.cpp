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

    if (player->GetState() == PlayerState::Attacking) {
        player->Attack(nullptr);
        return;
    }

    if (input.attack_basic){
        player->Attack(std::make_unique<EntityAttack>("Basic Attack", "Melee", 32.0f, 32.0f, 32.0f, 2, 1.28f));
    } else if (input.attack_2) {
        player->Attack(std::make_unique<EntityAttack>("Basic Attack 2", "Melee", 48.0f, 48.0f, 48.0f, 3, 1.28f));
    }

    player->RestoreActionPoints();
}