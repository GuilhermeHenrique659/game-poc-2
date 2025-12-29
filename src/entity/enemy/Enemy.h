#pragma once
#include "raylib.h"
#include "EnemyState.h"
#include "../player/Player.h"
#include "EnemyDirection.h"
#include <vector>
#include <optional>
#include "../common/EntityPosition.h"
#include "../common/EntityAttack.h"
#include "../common/Direction.h"
#include "../common/Entity.h"

class Enemy : public Entity
{
private:
    EnemyState enemyState = EnemyState::IDLE;
    EntityAttack *basicAttack;

public:
    int health = 5;

    Enemy(uint32_t id, Vector2 position, Rectangle destRec) : Entity("Enemy", id, new EntityPosition(position, Direction::DOWN, destRec, {position.x, position.y, destRec.width / 4, destRec.height / 4}, 3.0f)),
                                                              basicAttack(new EntityAttack(destRec.width * 0.5f, destRec.height * 0.5f, 60.0f, 0.8f, 1.28f)) {}
    ~Enemy() = default;

    EnemyState GetEnemyState();
    void SetEnemyState(EnemyState state);

    void Attack();

    void move(std::vector<Rectangle> &collisionRectangles, const std::vector<std::shared_ptr<Player>> players);

    bool IsHitted(Rectangle rect);
    void OnHit(Rectangle rect);

    std::optional<Rectangle> GetAttackHitbox();
};
