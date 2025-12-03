#include "Player.h"
#include "raymath.h"
#include "../../common/util/VectorUtil.h"
#include "../../config.h"

PlayerSpriteAnimation Player::GetPlayerSprite()
{
    return playerSpriteAnimation;
}

Direction Player::GetPlayerDirection()
{
    return playerPosition->GetDirection();
}

PlayerState Player::GetPlayerState()
{
    return playerState;
}

void Player::SetPlayerState(PlayerState state)
{
    if (state == ATTACK)
        basicAttack->attack(playerPosition);

    playerState = state;
}

Vector2 Player::GetPosition()
{
    return playerPosition->GetPostion();
}

Rectangle &Player::GetCollisionRectangle()
{
    return playerPosition->GetCollisionRectangle();
}

Rectangle Player::GetDestReactangle()
{
    return playerPosition->GetPositionRectangle();
}

void Player::UpdatePosition(Vector2 newPosition)
{
    playerPosition->UpdatePosition(newPosition);
}

bool Player::IsHitted(Rectangle rect)
{
    if (!basicAttack->getAttackbox().has_value())
        return false;

    return CheckCollisionRecs(basicAttack->getAttackbox().value(), rect);
}

void Player::OnHit(Rectangle rect)
{

    if (hitTimer > 0.0f)
    {
        hitTimer -= GetFrameTime();
        return;
    }

    if (CheckCollisionRecs(playerPosition->GetCollisionRectangle(), rect))
    {
        TraceLog(LOG_INFO, "Hitted");
        health -= 1;
        hitTimer = HIT_COOLDOWN; // inicia o cooldown
    }
}

std::optional<Rectangle> Player::GetAttackHitbox()
{
    return basicAttack->getAttackbox();
}

void Player::Attack()
{
    if (IsKeyDown(KEY_SPACE) && playerState != ATTACK)
    {
        playerState = ATTACK;
        playerSpriteAnimation.Reset();
        basicAttack->attack(playerPosition);

        _publish("player_attacked", std::any(basicAttack->getAttackbox()));
    }

    if (playerState == ATTACK && basicAttack->attack(playerPosition))
    {
        playerState = IDLE;
        playerSpriteAnimation.Reset();
    }
}

void Player::move(Camera2D camera, std::vector<Rectangle> collisionRectangles)
{

    Vector2 moveDir = {0, 0};

    if (IsKeyDown(KEY_UP))
    {
        moveDir.x += -1;
        moveDir.y += -1;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        moveDir.x += 1;
        moveDir.y += 1;
    }
    if (IsKeyDown(KEY_LEFT))
    {
        moveDir.x += -1;
        moveDir.y += 1;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        moveDir.x += 1;
        moveDir.y += -1;
    }

    if (playerPosition->MoveAndCollision(moveDir, collisionRectangles))
    {
        playerState = RUN;
        _publish("moved", std::any(playerPosition->GetPostion()));
    }
    else
    {
        playerState = IDLE;
    }
}

void Player::SetPlayerDirection(Direction newPlayerDirection)
{
    playerPosition->SetPlayerDirection(newPlayerDirection);
}

void Player::Animate()
{
    playerSpriteAnimation.Animate((PlayerDirection)playerPosition->GetDirection(), playerState);
}