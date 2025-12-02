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
    return enemyDirection;
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
    return position;
}

Rectangle &Enemy::GetCollisionRectangle()
{
    return collisionRectangle;
}

Rectangle Enemy::GetDestReactangle()
{
    return destRec;
}

void Enemy::UpdatePosition(Vector2 newPosition)
{
    position = newPosition;
    destRec.x = position.x;
    destRec.y = position.y;

    UpdateCollisionRectangle();
}

bool Enemy::IsHitted(Rectangle rect)
{
    if (!attackHitbox.has_value())
        return false;

    return CheckCollisionRecs(attackHitbox.value(), rect);
}

void Enemy::OnHit(Rectangle rect)
{
    if (CheckCollisionRecs(collisionRectangle, rect))
    {
        health -= 1;
    }
}

void Enemy::UpdateCollisionRectangle()
{
    collisionRectangle.x = position.x + (destRec.width * COLLISION_OFFSET_X);
    collisionRectangle.y = position.y + (destRec.height * COLLISION_OFFSET_Y);
    collisionRectangle.width = destRec.width * COLLISION_WIDTH;
    collisionRectangle.height = destRec.height * COLLISION_HEIGHT;
}

void Enemy::CreateAttackHitbox()
{
    switch (enemyDirection)
    {
    case EnemyDirection::RIGHT:
        attackHitbox = {position.x + destRec.width * 0.64f, position.y + (destRec.height * COLLISION_OFFSET_Y), 60.0f, destRec.height * COLLISION_HEIGHT};
        break;
    case EnemyDirection::LEFT:
        attackHitbox = {position.x + 72.0f, position.y + (destRec.height * COLLISION_OFFSET_Y), 60.0f, destRec.height * COLLISION_HEIGHT};
        break;
    case EnemyDirection::UP:
        attackHitbox = {position.x + (destRec.width * COLLISION_OFFSET_X), position.y + 40.0f, destRec.width * COLLISION_WIDTH, 60.0f};
        break;
    case EnemyDirection::DOWN:
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

Rectangle Enemy::GetFutureCollisionRectangle(Vector2 futurePosition) const
{
    Rectangle futureRect;
    futureRect.x = futurePosition.x + (destRec.width * COLLISION_OFFSET_X);
    futureRect.y = futurePosition.y + (destRec.height * COLLISION_OFFSET_Y);
    futureRect.width = destRec.width * COLLISION_WIDTH;
    futureRect.height = destRec.height * COLLISION_HEIGHT;
    return futureRect;
}

void Enemy::Attack()
{
    if (IsKeyDown(KEY_SPACE) && enemyState != EnemyState::ATTACK)
    {
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
        float d = Vector2DistanceSqr(position, p->GetPosition());
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
    Vector2 toTarget = Vector2Subtract(targetPos, position);
    float distance = Vector2Length(toTarget);

    const float STOP_DISTANCE = 40.0f; // para de tremer quando chega perto
    const float ATTACK_RANGE = 40.0f;  // distância para começar ataque
    const float ENEMY_SPEED = 3.9f;    // ajuste conforme seu jogo

    Vector2 oldPosition = position;
    Vector2 moveDir = {0, 0};

    // -------------------------------
    // PERSEGUIÇÃO
    // -------------------------------
    if (distance > STOP_DISTANCE)
    {
        // Direção normalizada no mundo cartesiano
        Vector2 dirCart = Vector2Scale(toTarget, 1.0f / distance);

        // Converte para isométrico (exatamente como você já fazia com as teclas)
        moveDir = ToIso(dirCart);
        NormalizeVectorInIso(moveDir);
        moveDir = Vector2Scale(moveDir, ENEMY_SPEED);

        // Posição futura para checar colisão
        Vector2 futurePos = Vector2Add(position, moveDir);
        Rectangle futureRect = GetFutureCollisionRectangle(futurePos);

        bool canMove = true;
        for (const auto &rect : collisionRectangles)
        {
            if (CheckCollisionRecs(futureRect, rect))
            {
                canMove = false;
                break;
            }
        }

        if (canMove)
            UpdatePosition(futurePos);
    }

    // -------------------------------
    // ATUALIZA ESTADO E DIREÇÃO (8 direções)
    // -------------------------------
    if (!Vector2Equals(oldPosition, position))
    {
        enemyState = EnemyState::RUN;

        // Calcula ângulo correto para animação (usa direção cartesiana do alvo)
        angle = RAD2DEG * atan2f(toTarget.y, toTarget.x);
        angle = fmodf(angle + 360.0f, 360.0f);
        enemyDirection = static_cast<EnemyDirection>(CalculateDirection());

        _publish("moved", std::any(position));
    }
    else
    {
        enemyState = EnemyState::IDLE;
    }

    // -------------------------------
    // ATAQUE AUTOMÁTICO QUANDO ESTIVER PERTO
    // -------------------------------
    if (distance <= ATTACK_RANGE && enemyState != EnemyState::ATTACK)
    {
        SetEnemyState(EnemyState::ATTACK); // já cria hitbox e reseta timer
        target->OnHit(attackHitbox.value());
        attackTimer = 0.0f;
    }
}

void Enemy::CalculateAngle(const Camera2D &camera, Vector2 &moveDir)
{
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
    {
        Vector2 mouseWorld = GetScreenToWorld2D(GetMousePosition(), camera);
        Vector2 toMouse = Vector2Subtract(mouseWorld, position);
        angle = RAD2DEG * atan2f(toMouse.y, toMouse.x);
    }
    else if (moveDir.x != 0.0f || moveDir.y != 0.0f)
    {
        angle = RAD2DEG * atan2f(moveDir.y, moveDir.x);
    }

    angle = fmodf(angle, 360.0f);
    if (angle < 0.0f)
        angle += 360.0f;
}

int Enemy::CalculateDirection()
{
    const float FORTY_FIVE_DEGREES = 45.0f;
    const float HALF_FORTY_FIVE_DEGREES = 22.5f;
    const int DIRECTIONS = 8;
    return (int)((angle + HALF_FORTY_FIVE_DEGREES) / FORTY_FIVE_DEGREES) % DIRECTIONS;
}

void Enemy::SetEnemyDirection(EnemyDirection newEnemyDirection)
{
    enemyDirection = newEnemyDirection;
}

void Enemy::Animate()
{
    enemySpriteAnimation.Animate(enemyDirection, enemyState);
}

Vector2 Enemy::NormalizeMove(Vector2 &moveDir)
{
    const float speed = 5.0f;
    moveDir = ToIso(moveDir);

    if (moveDir.x == 0 && moveDir.y == 0)
        return Vector2{
            position.x,
            position.y};

    NormalizeVectorInIso(moveDir);

    return Vector2{
        position.x + moveDir.x * speed,
        position.y + moveDir.y * speed};
}