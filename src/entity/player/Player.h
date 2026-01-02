#pragma once
#include "PlayerState.h"

#include "../common/Entity.h"
#include "../common/EntityPosition.h"
#include "../common/EntityAttack.h"
#include "../../common/CollisionLines.h"

class Player : public Entity
{
private:
    PlayerState current_state;
    EntityAttack entity_attack;
    int attack_count = 0;

public:
    Player(uint32_t id, std::string label, std::unique_ptr<EntityPosition> entityPosition) : Entity(label, id, std::move(entityPosition)),
                                                                                             current_state(PlayerState::Idle),
                                                                                             entity_attack(EntityAttack(
                                                                                                 20.0f,
                                                                                                 20.0f,
                                                                                                 60.0f,
                                                                                                 0.8f,
                                                                                                 1.28f)) {};
    ~Player() = default;

    void ChangeState(PlayerState new_state);
    void Move(Vector2 move_direction, std::vector<CollisionLines> collision_lines);
    void Attack();

    PlayerState GetState() const;
};