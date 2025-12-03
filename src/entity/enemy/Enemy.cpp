#include "Enemy.h"
#include "raymath.h"
#include <float.h>
#include "../../common/util/VectorUtil.h"
#include "../../config.h"

EnemySpriteAnimation Enemy::GetEnemySprite()
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
        CreateAttackHitbox();

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
    if (!attackHitbox.has_value())
        return false;

    return CheckCollisionRecs(attackHitbox.value(), rect);
}

void Enemy::OnHit(Rectangle rect)
{
    if (CheckCollisionRecs(enemyPosition->GetCollisionRectangle(), rect))
    {
        TraceLog(LOG_INFO, "Enemy hitted");
        health -= 1;
    }
}

void Enemy::CreateAttackHitbox()
{
    Vector2 position = enemyPosition->GetPostion();
    Rectangle destRec = enemyPosition->GetPositionRectangle();

    switch (enemyPosition->GetDirection())
    {
    case Direction::RIGHT:
        attackHitbox = {position.x + destRec.width * 0.64f, position.y + (destRec.height * COLLISION_OFFSET_Y), 60.0f, destRec.height * COLLISION_HEIGHT};
        break;
    case Direction::LEFT:
        attackHitbox = {position.x + 72.0f, position.y + (destRec.height * COLLISION_OFFSET_Y), 60.0f, destRec.height * COLLISION_HEIGHT};
        break;
    case Direction::UP:
        attackHitbox = {position.x + (destRec.width * COLLISION_OFFSET_X), position.y + 40.0f, destRec.width * COLLISION_WIDTH, 60.0f};
        break;
    case Direction::DOWN:
        attackHitbox = {position.x + (destRec.width * COLLISION_OFFSET_X), position.y + destRec.height * 0.64f, destRec.width * COLLISION_WIDTH, 60.0f};
        break;
    default:
        attackHitbox = {position.x + destRec.width * 0.64f,
                        position.y + (destRec.height * COLLISION_OFFSET_Y),
                        60.0f,
                        destRec.height * COLLISION_HEIGHT};
        break;
    }
}

std::optional<Rectangle> Enemy::GetAttackHitbox()
{
    return attackHitbox;
}

void Enemy::Attack()
{
    if (enemyState != EnemyState::ATTACK)
    {
        attackHitbox.reset();
        enemyState = EnemyState::ATTACK;
        attackTimer = 0.0f;
        enemySpriteAnimation.Reset();
        CreateAttackHitbox();
    }

    if (enemyState == EnemyState::ATTACK)
    {
        attackTimer += GetFrameTime();

        if (attackTimer >= ATTACK_DURATION)
        {
            enemyState = EnemyState::IDLE;
            attackTimer = 0.0f;
            attackHitbox.reset();
            enemySpriteAnimation.Reset();

            return;
        }
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
        if (attackHitbox.has_value())
            target->OnHit(attackHitbox.value());
    }
}

void Enemy::SetEnemyDirection(EnemyDirection newEnemyDirection)
{
    enemyPosition->SetPlayerDirection((Direction)newEnemyDirection);
}

void Enemy::Animate()
{
    enemySpriteAnimation.Animate((EnemyDirection)enemyPosition->GetDirection(), enemyState);
}