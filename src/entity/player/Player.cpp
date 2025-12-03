#include "Player.h"
#include "raymath.h"
#include "../../common/util/VectorUtil.h"
#include "../../config.h"

PlayerSpriteAnimation Player::GetPlayerSprite()
{
    return playerSpriteAnimation;
}

PlayerDirection Player::GetPlayerDirection()
{
    return (PlayerDirection)playerPosition->GetDirection();
}

PlayerState Player::GetPlayerState()
{
    return playerState;
}

void Player::SetPlayerState(PlayerState state)
{
    if (state == ATTACK)
        CreateAttackHitbox();

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
    if (!attackHitbox.has_value())
        return false;

    return CheckCollisionRecs(attackHitbox.value(), rect);
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

void Player::CreateAttackHitbox()
{
    Vector2 position = playerPosition->GetPostion();
    Rectangle destRec = playerPosition->GetPositionRectangle();

    switch (playerPosition->GetDirection())
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

std::optional<Rectangle> Player::GetAttackHitbox()
{
    return attackHitbox;
}

void Player::Attack()
{
    if (IsKeyDown(KEY_SPACE) && playerState != ATTACK)
    {
        playerState = ATTACK;
        attackTimer = 0.0f;
        playerSpriteAnimation.Reset();
        CreateAttackHitbox();

        _publish("player_attacked", std::any(attackHitbox));
    }

    if (playerState == ATTACK)
    {
        attackTimer += GetFrameTime();

        if (attackTimer >= ATTACK_DURATION)
        {
            playerState = PlayerState::IDLE;
            attackTimer = 0.0f;
            attackHitbox.reset();
            playerSpriteAnimation.Reset();

            return;
        }
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

void Player::SetPlayerDirection(PlayerDirection newPlayerDirection)
{
    playerPosition->SetPlayerDirection((Direction)newPlayerDirection);
}

void Player::Animate()
{
    playerSpriteAnimation.Animate((PlayerDirection)playerPosition->GetDirection(), playerState);
}