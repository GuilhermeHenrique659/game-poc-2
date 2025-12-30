#include "Player.h"

const char *StateToString(PlayerState state)
{
    switch (state)
    {
    case PlayerState::Idle:
        return "IDLE";
    case PlayerState::Running:
        return "WALKING";
    case PlayerState::Attacking:
        return "ATTACKING";
    default:
        return "UNKNOWN";
    }
}

void Player::ChangeState(PlayerState new_state)
{
    if (current_state == new_state)
        return;

    TraceLog(LOG_INFO, "Player %d changing state from %s to %s", id, StateToString(current_state), StateToString(new_state));

    current_state = new_state;

    _publish("state_change", static_cast<int>(current_state));
}

PlayerState Player::GetState() const
{
    return current_state;
}

void Player::Move(Vector2 move_direction, std::vector<CollisionLines> collision_lines)
{
    if (current_state == PlayerState::Attacking)
        return;

    if (entityPosition->MoveAndCollision(move_direction, collision_lines))
    {
        ChangeState(PlayerState::Running);
    }
    else
    {
        ChangeState(PlayerState::Idle);
    }
}

void Player::Attack()
{
    if (current_state != PlayerState::Attacking)
    {
        TraceLog(LOG_INFO, "Player %d attacking", id);
        entity_attack.attack(entityPosition.get());
        ChangeState(PlayerState::Attacking);
    }

    if (current_state == PlayerState::Attacking && entity_attack.attack(entityPosition.get()))
    {
        TraceLog(LOG_INFO, "Player %d finished attacking", id);
        ChangeState(PlayerState::Idle);
    }
}