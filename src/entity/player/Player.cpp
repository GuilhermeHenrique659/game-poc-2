#include "Player.h"

void Player::ChangeState(PlayerState new_state)
{
    if (current_state == new_state)
        return;

    current_state = new_state;

    _publish("state_change", static_cast<int>(current_state));
}

PlayerState Player::GetState() const
{
    return current_state;
}

void Player::Move(Vector2 move_direction, std::vector<Rectangle> collision_rectangles)
{
    if (entityPosition->MoveAndCollision(move_direction, collision_rectangles))
    {
        ChangeState(PlayerState::Running);
    }
    else
    {
        ChangeState(PlayerState::Idle);
    }
}