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
#include "../common/EntityAnimationSprite.cpp"

class Enemy : public Entity
{
private:
    EnemyState enemyState = EnemyState::IDLE;
    EntityPosition *enemyPosition;
    EntityAttack *basicAttack;

    EntityAnimationSprite enemySpriteAnimation;

public:
    int health = 5;

    Enemy(uint32_t id, Vector2 position, Rectangle destRec) : Entity("Enemy", id),
                                                              enemySpriteAnimation(EntityAnimationSprite()),
                                                              enemyPosition(new EntityPosition(position, Direction::DOWN, destRec, {position.x, position.y, destRec.width / 4, destRec.height / 4}, 3.0f)),
                                                              basicAttack(new EntityAttack(destRec.width * 0.5f, destRec.height * 0.5f, 60.0f, 0.8f, 1.28f)) {}
    ~Enemy() = default;

    Rectangle GetDestReactangle();
    Vector2 GetPosition();
    EntityAnimationSprite GetEnemySprite();

    EnemyState GetEnemyState();
    void SetEnemyState(EnemyState state);

    EnemyDirection GetEnemyDirection();
    void SetEnemyDirection(EnemyDirection enemyDirection);

    void UpdatePosition(Vector2 position);

    void Animate();
    void Attack();

    void move(std::vector<Rectangle> &collisionRectangles, const std::vector<std::shared_ptr<Player>> players);

    bool IsHitted(Rectangle rect);
    void OnHit(Rectangle rect);

    std::optional<Rectangle> GetAttackHitbox();

    Rectangle &GetCollisionRectangle();
};
