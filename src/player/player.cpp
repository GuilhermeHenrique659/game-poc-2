#include "Player.h"
#include "raymath.h"

void Player::move(Camera2D camera)
{
    Vector2 oldPosition = position;
    Vector2 moveDir = {0.0f, 0.0f};

    if (IsKeyDown(KEY_RIGHT))
        moveDir.x += 1.0f;
    if (IsKeyDown(KEY_LEFT))
        moveDir.x -= 1.0f;
    if (IsKeyDown(KEY_UP))
        moveDir.y -= 1.0f;
    if (IsKeyDown(KEY_DOWN))
        moveDir.y += 1.0f;

    NormalizeMove(moveDir);

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

    destRec.x = position.x - destRec.width * 0.5f;
    destRec.y = position.y - destRec.height * 0.5f;

    if (!Vector2Equals(oldPosition, position))
    {
        runningTime += GetFrameTime();
        if (runningTime >= 0.08f) // velocidade da animação (8 frames = ~0.64s total)
        {
            runningTime = 0.0f;
            currentFrame = (currentFrame + 1) % 10; // 8 frames de corrida
        }

        isIdle = false;
        _publish("moved", std::any(position));
    }
    else
        isIdle = true;
}

void Player::NormalizeMove(Vector2 &moveDir)
{
    float OneOverSqrt2 = 0.7071f; // 1/sqrt(2)

    if (moveDir.x != 0.0f || moveDir.y != 0.0f)
    {
        if (moveDir.x != 0.0f && moveDir.y != 0.0f)
        {
            moveDir.x *= OneOverSqrt2; // 1/sqrt(2)
            moveDir.y *= OneOverSqrt2;
        }
        position.x += moveDir.x * 5.0f; // supondo que tenha float speed = 200.0f ou algo assim
        position.y += moveDir.y * 5.0f;

        Vector2Normalize(position);
    }
}
