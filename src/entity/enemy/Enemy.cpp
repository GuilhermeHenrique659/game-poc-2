#include "Enemy.h"
#include "raymath.h"
#include <float.h>
#include "../../common/util/VectorUtil.h"
#include "../../config.h"
#include "EnemySpriteAnimation.cpp"

EntityAnimationSprite Enemy::GetEnemySprite()
{
    return enemySpriteAnimation;
}

EnemyDirection Enemy::GetEnemyDirection()
{
    return (EnemyDirection)enemyPosition->GetDirection();
}

EnemyState Enemy::GetEnemyState()
{
    return enemyState;
}

void Enemy::SetEnemyState(EnemyState state)
{
    if (state == EnemyState::ATTACK)
        basicAttack->attack(enemyPosition);

    enemyState = state;
}

Vector2 Enemy::GetPosition()
{
    return enemyPosition->GetPostion();
}

Rectangle &Enemy::GetCollisionRectangle()
{
    return enemyPosition->GetCollisionRectangle();
}

Rectangle Enemy::GetDestReactangle()
{
    return enemyPosition->GetPositionRectangle();
}

bool Enemy::IsHitted(Rectangle rect)
{
    if (!basicAttack->getAttackbox().has_value())
        return false;

    return CheckCollisionRecs(basicAttack->getAttackbox().value(), rect);
}

void Enemy::OnHit(Rectangle rect)
{
    if (CheckCollisionRecs(enemyPosition->GetCollisionRectangle(), rect))
    {
        TraceLog(LOG_INFO, "Enemy hitted");
        health -= 1;
    }
}

std::optional<Rectangle> Enemy::GetAttackHitbox()
{
    return basicAttack->getAttackbox();
}

void Enemy::Attack()
{
    if (enemyState != EnemyState::ATTACK)
    {
        enemyState = EnemyState::ATTACK;
        enemySpriteAnimation.Reset();
        basicAttack->attack(enemyPosition);

        _publish("player_attacked", std::any(basicAttack->getAttackbox()));
    }

    if (enemyState == EnemyState::ATTACK && basicAttack->attack(enemyPosition))
    {
        enemyState = EnemyState::IDLE;
        enemySpriteAnimation.Reset();
    }
}

void Enemy::UpdatePosition(Vector2 newPostion)
{
    enemyPosition->UpdatePosition(newPostion);
}

void Enemy::move(std::vector<Rectangle> &collisionRectangles, const std::vector<std::shared_ptr<Player>> players)
{
    if (players.empty())
    {
        enemyState = EnemyState::IDLE;
        return;
    }

    // 1. Encontra o player mais próximo
    std::shared_ptr<Player> target = nullptr;
    float closestDistSqr = FLT_MAX;

    for (auto &p : players)
    {
        float d = Vector2DistanceSqr(enemyPosition->GetPostion(), p->GetPosition());
        if (d < closestDistSqr)
        {
            closestDistSqr = d;
            target = p;
        }
    }

    if (!target)
    {
        enemyState = EnemyState::IDLE;
        return;
    }

    Vector2 targetPos = target->GetPosition();
    Vector2 toTarget = Vector2Subtract(targetPos, enemyPosition->GetPostion());
    float distance = Vector2Length(toTarget);

    const float STOP_DISTANCE = 50.0f; // para de tremer quando chega perto
    const float ATTACK_RANGE = 60.0f;  // distância para começar ataque

    if (distance > STOP_DISTANCE)
    {

        Vector2 moveDir = Vector2Scale(toTarget, 1.0f / distance);
        // -------------------------------
        // PERSEGUIÇÃO
        // -------------------------------
        if (enemyPosition->MoveAndCollision(moveDir, collisionRectangles))
        {
            enemyState = EnemyState::RUN;

            _publish("moved", std::any(enemyPosition->GetPostion()));
        }
        else
        {
            enemyState = EnemyState::IDLE;
        }
    }

    if (distance <= ATTACK_RANGE)
    {
        Attack();
        if (basicAttack->getAttackbox().has_value())
            target->OnHit(basicAttack->getAttackbox().value());
    }
}

void Enemy::SetEnemyDirection(EnemyDirection newEnemyDirection)
{
    enemyPosition->SetPlayerDirection((Direction)newEnemyDirection);
}

void Enemy::Animate()
{
    enemySpriteAnimation.Animate(enemyPosition->GetDirection(), GetByState(enemyState));
}