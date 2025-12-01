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
    return playerDirection;
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
    return position;
}

Rectangle &Player::GetCollisionRectangle()
{
    return collisionRectangle;
}

Rectangle Player::GetDestReactangle()
{
    return destRec;
}

void Player::UpdatePosition(Vector2 newPosition)
{
    position = newPosition;
    destRec.x = position.x;
    destRec.y = position.y;

    UpdateCollisionRectangle();
}

bool Player::IsHitted(Rectangle rect)
{
    if (!attackHitbox.has_value())
        return false;

    return CheckCollisionRecs(attackHitbox.value(), rect);
}

void Player::UpdateCollisionRectangle()
{
    collisionRectangle.x = position.x + (destRec.width * COLLISION_OFFSET_X);
    collisionRectangle.y = position.y + (destRec.height * COLLISION_OFFSET_Y);
    collisionRectangle.width = destRec.width * COLLISION_WIDTH;
    collisionRectangle.height = destRec.height * COLLISION_HEIGHT;
}

void Player::CreateAttackHitbox()
{
    switch (playerDirection)
    {
    case PlayerDirection::RIGHT:
        attackHitbox = {position.x + destRec.width * 0.64f, position.y + (destRec.height * COLLISION_OFFSET_Y), 60.0f, destRec.height * COLLISION_HEIGHT};
        break;
    case PlayerDirection::LEFT:
        attackHitbox = {position.x + 72.0f, position.y + (destRec.height * COLLISION_OFFSET_Y), 60.0f, destRec.height * COLLISION_HEIGHT};
        break;
    case PlayerDirection::UP:
        attackHitbox = {position.x + (destRec.width * COLLISION_OFFSET_X), position.y + 40.0f, destRec.width * COLLISION_WIDTH, 60.0f};
        break;
    case PlayerDirection::DOWN:
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

Rectangle Player::GetFutureCollisionRectangle(Vector2 futurePosition) const
{
    Rectangle futureRect;
    futureRect.x = futurePosition.x + (destRec.width * COLLISION_OFFSET_X);
    futureRect.y = futurePosition.y + (destRec.height * COLLISION_OFFSET_Y);
    futureRect.width = destRec.width * COLLISION_WIDTH;
    futureRect.height = destRec.height * COLLISION_HEIGHT;
    return futureRect;
}

void Player::Attack()
{
    if (IsKeyDown(KEY_SPACE) && playerState != ATTACK)
    {
        playerState = ATTACK;
        attackTimer = 0.0f;
        playerSpriteAnimation.Reset();
        CreateAttackHitbox();
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

    Vector2 oldPosition = position;
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

    auto newPosition = NormalizeMove(moveDir);

    CalculateAngle(camera, moveDir);

    playerDirection = static_cast<PlayerDirection>(CalculateDirection());
    Rectangle futureCollisionRect = GetFutureCollisionRectangle(newPosition);

    bool canMove = true;
    for (const auto &rect : collisionRectangles)
    {
        if (CheckCollisionRecs(futureCollisionRect, rect))
        {
            canMove = false;
            break;
        }
    }

    if (canMove)
    {
        UpdatePosition(newPosition);
    }

    if (!Vector2Equals(oldPosition, position))
    {
        playerState = RUN;
        _publish("moved", std::any(position));
    }
    else
    {
        playerState = IDLE;
    }
}

void Player::CalculateAngle(const Camera2D &camera, Vector2 &moveDir)
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

int Player::CalculateDirection()
{
    const float FORTY_FIVE_DEGREES = 45.0f;
    const float HALF_FORTY_FIVE_DEGREES = 22.5f;
    const int DIRECTIONS = 8;
    return (int)((angle + HALF_FORTY_FIVE_DEGREES) / FORTY_FIVE_DEGREES) % DIRECTIONS;
}

void Player::SetPlayerDirection(PlayerDirection newPlayerDirection)
{
    playerDirection = newPlayerDirection;
}

void Player::Animate()
{
    playerSpriteAnimation.Animate(playerDirection, playerState);
}

Vector2 Player::NormalizeMove(Vector2 &moveDir)
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