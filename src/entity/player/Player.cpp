#include "Player.h"
#include "raymath.h"
#include "../../common/util/VectorUtil.h"

PlayerSpriteAnimation Player::GetPlayerSprite()
{
    return playerSpriteAnimation;
}

PlayerDirection Player::GetPlayerDirection()
{
    return playerDirection;
}

void Player::move(Camera2D camera)
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

    NormalizeMove(moveDir);

    CalculateAngle(camera, moveDir);

    playerDirection = static_cast<PlayerDirection>(CalculateDirection());

    destRec.x = position.x - destRec.width * 0.5f;
    destRec.y = position.y - destRec.height * 0.5f;

    if (!Vector2Equals(oldPosition, position))
    {
        isIdle = false;
        _publish("moved", std::any(position));
    }
    else
    {
        isIdle = true;
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
    playerSpriteAnimation.Animate(playerDirection, isIdle);
}

void Player::NormalizeMove(Vector2 &moveDir)
{
    const float speed = 5.0f;
    moveDir = ToIso(moveDir);

    if (moveDir.x == 0 && moveDir.y == 0)
        return;

    NormalizeVectorInIso(moveDir);

    position.x += moveDir.x * speed;
    position.y += moveDir.y * speed;
}