#pragma once
#include "../common/Entity.h"
#include "../common/EntityPosition.h"
#include "PlayerState.h"

class Player : public Entity
{
private:
    PlayerState current_state;

public:
    Player(uint32_t id, std::string label, std::unique_ptr<EntityPosition> entityPosition) : Entity(label, id, std::move(entityPosition)), current_state(PlayerState::Idle) {};
    ~Player() = default;

    void ChangeState(PlayerState new_state);
    void Move(Vector2 move_direction, std::vector<Rectangle> collision_rectangles);

    PlayerState GetState() const;
};