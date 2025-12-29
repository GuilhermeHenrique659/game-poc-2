#include "Enemy.h"
#include "raymath.h"
#include <float.h>
#include "../../common/util/VectorUtil.h"
#include "../../config.h"

EnemyState Enemy::GetEnemyState()
{
    return enemyState;
}

void Enemy::SetEnemyState(EnemyState state)
{
    if (state == enemyState)
        return;

    enemyState = state;

    _publish("state_changed", std::any(enemyState));
}

bool Enemy::IsHitted(Rectangle rect)
{
    if (!basicAttack->getAttackbox().has_value())
        return false;

    return CheckCollisionRecs(basicAttack->getAttackbox().value(), rect);
}

void Enemy::OnHit(Rectangle rect)
{
    if (CheckCollisionRecs(entityPosition->GetCollisionRectangle(), rect))
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
        SetEnemyState(EnemyState::ATTACK);
        basicAttack->attack(entityPosition);

        _publish("player_attacked", std::any(basicAttack->getAttackbox()));
    }

    if (enemyState == EnemyState::ATTACK && basicAttack->attack(entityPosition))
    {
        SetEnemyState(EnemyState::IDLE);
    }
}

void Enemy::move(std::vector<Rectangle> &collisionRectangles, const std::vector<std::shared_ptr<Player>> players)
{
    if (players.empty())
    {
        SetEnemyState(EnemyState::IDLE);
        return;
    }

    // 1. Encontra o player mais próximo
    std::shared_ptr<Player> target = nullptr;
    float closestDistSqr = FLT_MAX;

    for (auto &p : players)
    {
        float d = Vector2DistanceSqr(entityPosition->GetPostion(), p->GetPosition());
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
    Vector2 toTarget = Vector2Subtract(targetPos, entityPosition->GetPostion());
    float distance = Vector2Length(toTarget);

    const float STOP_DISTANCE = 50.0f; // para de tremer quando chega perto
    const float ATTACK_RANGE = 60.0f;  // distância para começar ataque

    if (distance > STOP_DISTANCE)
    {

        Vector2 moveDir = Vector2Scale(toTarget, 1.0f / distance);
        // -------------------------------
        // PERSEGUIÇÃO
        // -------------------------------
        if (entityPosition->MoveAndCollision(moveDir, collisionRectangles))
        {
            SetEnemyState(EnemyState::RUN);
            ;

            _publish("moved", std::any(entityPosition->GetPostion()));
        }
        else
        {
            SetEnemyState(EnemyState::IDLE);
        }
    }

    if (distance <= ATTACK_RANGE)
    {
        Attack();
        if (basicAttack->getAttackbox().has_value())
            target->OnHit(basicAttack->getAttackbox().value());
    }
}