#pragma once
#include "../../common/Subject.h"
#include "raylib.h"
#include "EnemySpriteAnimation.h"
#include "EnemyState.h"
#include "../player/Player.h"
#include <vector>
#include <optional>
#include "../common/EntityPosition.h"
#include "../common/EntityAttack.h"

class Enemy : public Subject
{
private:
    EnemyState enemyState = EnemyState::IDLE;
    EntityPosition *enemyPosition;
    EntityAttack *basicAttack;

    EnemySpriteAnimation enemySpriteAnimation;

public:
    int health = 5;

    Enemy(Vector2 position, Rectangle destRec, EnemySpriteAnimation enemySpriteAnimation) : enemySpriteAnimation(enemySpriteAnimation),
                                                                                            enemyPosition(new EntityPosition(position, Direction::DOWN, destRec, {position.x, position.y, destRec.width / 4, destRec.height / 4}, 3.0f)),
                                                                                            basicAttack(new EntityAttack(destRec.width * 0.5f, destRec.height * 0.5f, 60.0f, 0.8f, 1.28f)) {};
    ~Enemy() = default;

    Rectangle GetDestReactangle();
    Vector2 GetPosition();
    EnemySpriteAnimation GetEnemySprite();

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
