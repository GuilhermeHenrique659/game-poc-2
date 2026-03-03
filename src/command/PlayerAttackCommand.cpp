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

    player->RestoreActionPoints();
    //TODO: REFACTOR HERE

    if (input.attack_basic || input.attack_2 || player->GetState() == PlayerState::Attacking){
        std::string attack_name = "Basic Attack";
        if (input.attack_2) {
            attack_name = "Basic Attack 2";
        }

        player->Attack(std::make_unique<EntityAttack>(attack_name, "Melee", 32.0f, 32.0f, 32.0f, 2, 1.28f));
    }
}