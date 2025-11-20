#include "Enemy.h"

Enemy::Enemy(Vector2 position)
{
    this->position = position;
}

Enemy::~Enemy() = default;

void Enemy::follow(Vector2 target)
{
    if (target.x > position.x)
    {
        position.x += 2.1f;
    }
    if (target.x < position.x)
    {
        position.x -= 2.1f;
    }
    if (target.y > position.y)
    {
        position.y += 2.1f;
    }
    if (target.y < position.y)
    {
        position.y -= 2.1f;
    }
}