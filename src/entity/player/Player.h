#pragma once
#include <memory>
#include <string.h>
#include <optional>
#include "PlayerState.h"

#include "../common/Entity.h"
#include "../common/EntityPosition.h"
#include "../common/EntityAttack.h"
#include "../../common/CollisionLines.h"

struct ActionPoints {
    int points;
    int const max_points;
    float restore_timer_accumlator;
};

struct Health
{
    int current_health;
    int max_health;
};




class Player : public Entity
{
private:
    PlayerState current_state;
    std::unique_ptr<EntityAttack> current_attack;
    int attack_count = 0;
    Health health = {10, 10};
    ActionPoints action_points = {6, 6, 0.0f};

public:
    Player(uint32_t id, std::string label, std::unique_ptr<EntityPosition> entityPosition) : Entity(label, id, std::move(entityPosition)),
                                                                                             current_state(PlayerState::Idle),
                                                                                             current_attack(nullptr) {};
    ~Player() = default;

    void SetHealth(int health_points);
    void SetActionPoints(int points);

    void ChangeState(PlayerState new_state);
    void Move(Vector2 move_direction, std::vector<CollisionLines> collision_lines);
    void Attack(std::unique_ptr<EntityAttack> attack);
    void SetCurrentAttack(std::unique_ptr<EntityAttack> attack);

    void RestoreActionPoints();

    static std::shared_ptr<Player> Create(uint32_t id, Vector2 position, Direction direction);
    PlayerState GetState() const;
    const Health& GetHealth() const;
    const ActionPoints& GetActionPoints() const;

    const std::optional<std::string> GetCurrentAttackName() const;
};