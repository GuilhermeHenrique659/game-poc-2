#include "../../common/util/Timer.h"
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

void Player::SetHealth(int health_points)
{
    health.current_health = std::min(health_points, health.max_health);
}

void Player::SetActionPoints(int points)
{
    action_points.points = std::min(points, action_points.max_points);
}

void Player::RestoreActionPoints()
{
    float restore_points_interval = 1.0f; // 1 segundo

    RunEvery(&action_points.restore_timer_accumlator, restore_points_interval, [this]() {
        if (this->action_points.points < this->action_points.max_points) {
            this->action_points.points++;
            TraceLog(LOG_INFO, "Player %d restored points: %d", 
                     this->id, this->action_points.points);
        }
    });
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

void Player::Attack(std::unique_ptr<EntityAttack> attack)
{
    if (!current_attack.has_value()) {
        current_attack = std::move(attack);
    }

    if (current_state != PlayerState::Attacking)
    {
        if (action_points.points < current_attack.value()->GetAttackPointsConsume()) return;
        TraceLog(LOG_INFO, "Player %d attacking", id);
        current_attack.value()->attack(entityPosition.get());
        ChangeState(PlayerState::Attacking);

        action_points.points-=current_attack.value()->GetAttackPointsConsume();
    }

    if (current_state == PlayerState::Attacking && current_attack.value()->attack(entityPosition.get()))
    {
        TraceLog(LOG_INFO, "Player %d finished attacking", id);
        ChangeState(PlayerState::Idle);
        current_attack.reset();
    }
}

std::shared_ptr<Player> Player::Create(uint32_t id, Vector2 position, Direction direction)
{
    auto player_position = std::make_unique<EntityPosition>(position, direction, Rectangle{position.y, position.y, 320.0f, 320.0f}, Rectangle{}, 6.0f);

    return std::make_shared<Player>(id, "Player", std::move(player_position));
}

const Health& Player::GetHealth() const
{
    return health;
}

const ActionPoints& Player::GetActionPoints() const
{
    return action_points;
}