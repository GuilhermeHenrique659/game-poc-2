#pragma once
#include "../common/Entity.h"
#include "../common/EntityPosition.h"
#include "../common/EntityAttack.h"
#include "PlayerState.h"

class Player : public Entity
{
private:
    PlayerState current_state;
    EntityAttack entity_attack;

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
    void Move(Vector2 move_direction, std::vector<Rectangle> collision_rectangles);
    void Attack();

    PlayerState GetState() const;
};