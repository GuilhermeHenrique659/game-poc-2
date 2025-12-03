#include "EntityPosition.h"
#include "raymath.h"
#include "../../common/util/VectorUtil.h"
#include "../../config.h"

float EntityPosition::CalculateAngle(Vector2 &moveDir)
{
    float angle = 0;
    if (moveDir.x != 0.0f || moveDir.y != 0.0f)
    {
        angle = RAD2DEG * atan2f(moveDir.y, moveDir.x);
    }

    angle = fmodf(angle, 360.0f);
    if (angle < 0.0f)
        angle += 360.0f;

    return angle;
}

Rectangle EntityPosition::GetFutureCollisionRectangle(Vector2 futurePosition) const
{
    Rectangle futureRect;
    futureRect.x = futurePosition.x + (positionRectangle.width * COLLISION_OFFSET_X);
    futureRect.y = futurePosition.y + (positionRectangle.height * COLLISION_OFFSET_Y);
    futureRect.width = positionRectangle.width * COLLISION_WIDTH;
    futureRect.height = positionRectangle.height * COLLISION_HEIGHT;
    return futureRect;
}

Direction EntityPosition::CalculateDirection(float angle)
{
    const float FORTY_FIVE_DEGREES = 45.0f;
    const float HALF_FORTY_FIVE_DEGREES = 22.5f;
    const int DIRECTIONS = 8;
    int direction = (int)((angle + HALF_FORTY_FIVE_DEGREES) / FORTY_FIVE_DEGREES) % DIRECTIONS;

    return (Direction)direction;
}

Vector2 EntityPosition::NormalizeMove(Vector2 &moveDir)
{
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

bool EntityPosition::MoveAndCollision(Vector2 moveDirection, std::vector<Rectangle> collisionRectangles)
{

    auto newPosition = NormalizeMove(moveDirection);

    if (Vector2Equals(position, newPosition))
        return false;

    float angle = CalculateAngle(moveDirection);

    direction = CalculateDirection(angle);
    Rectangle futureCollisionRect = GetFutureCollisionRectangle(newPosition);

    for (const auto &rect : collisionRectangles)
    {
        if (CheckCollisionRecs(futureCollisionRect, rect))
        {
            return false;
        }
    }

    UpdatePosition(newPosition);

    return true;
}

void EntityPosition::UpdatePosition(Vector2 newPosition)
{
    position = newPosition;
    positionRectangle.x = position.x;
    positionRectangle.y = position.y;

    collisionRectangle.x = position.x + (positionRectangle.width * COLLISION_OFFSET_X);
    collisionRectangle.y = position.y + (positionRectangle.height * COLLISION_OFFSET_Y);
    collisionRectangle.width = positionRectangle.width * COLLISION_WIDTH;
    collisionRectangle.height = positionRectangle.height * COLLISION_HEIGHT;
}