#pragma once
#include "../../common/Subject.h"
#include "raylib.h"
#include "EnemySpriteAnimation.h"
#include "EnemyState.h"
#include "../player/Player.h"
#include <vector>
#include <optional>

class Enemy : public Subject
{
private:
    EnemyDirection enemyDirection = EnemyDirection::DOWN;
    EnemyState enemyState = EnemyState::IDLE;

    EnemySpriteAnimation enemySpriteAnimation;
    Vector2 position;
    Rectangle destRec;
    Rectangle collisionRectangle;
    std::optional<Rectangle> attackHitbox;

    int health = 10;

    float attackTimer = 0.0f;
    float attackCooldown = 0.8f;

    Vector2 NormalizeMove(Vector2 &moveDir);

public:
    float angle = 0;

    Enemy(Vector2 position, Rectangle destRec, EnemySpriteAnimation enemySpriteAnimation) : position(position), destRec(destRec), enemySpriteAnimation(enemySpriteAnimation),
                                                                                            collisionRectangle{position.x, position.y, destRec.width / 4, destRec.height / 4} {};
    ~Enemy() = default;

    Rectangle GetDestReactangle();
    Vector2 GetPosition();
    EnemySpriteAnimation GetEnemySprite();

    EnemyState GetEnemyState();
    void SetEnemyState(EnemyState state);

    EnemyDirection GetEnemyDirection();
    void SetEnemyDirection(EnemyDirection enemyDirection);

    void UpdatePosition(Vector2 position);

    void CalculateAngle(const Camera2D &camera, Vector2 &moveDir);
    int CalculateDirection();

    void Animate();
    void Attack();

    void move(std::vector<Rectangle> &collisionRectangles, const std::vector<std::shared_ptr<Player>> players);

    bool IsHitted(Rectangle rect);
    void OnHit(Rectangle rect);

    void CreateAttackHitbox();
    std::optional<Rectangle> GetAttackHitbox();

    void UpdateCollisionRectangle();

    Rectangle &GetCollisionRectangle();

    Rectangle GetFutureCollisionRectangle(Vector2 futurePosition) const;
};
