#include "EntityPosition.h"
#include "raymath.h"
#include "../../common/util/VectorUtil.h"
#include "../../config.h"

Vector2 EntityPosition::GetEntityFeet()
{
    return entity_feet;
}

Vector2 EntityPosition::CalculateEntityFeet(Vector2 new_position)
{
    return {new_position.x + (positionRectangle.width / 2.0f), new_position.y + (positionRectangle.height * 0.6f)};
}

bool EntityPosition::IsColliding(Vector2 position, std::vector<CollisionLines> collision_lines)
{
    entity_feet = CalculateEntityFeet(position);
    float radius = (positionRectangle.width * 0.1f);
    for (const auto &line : collision_lines)
    {
        if (CheckCollisionCircleLine(entity_feet, radius, line.start, line.end))
            return true;
    }

    return false;
}

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

bool EntityPosition::MoveAndCollision(Vector2 moveDirection, std::vector<CollisionLines> collision_lines)
{
    Vector2 old_position = position;
    Vector2 nextPos = NormalizeMove(moveDirection);
    if (Vector2Equals(position, nextPos))
        return false;

    Vector2 nextPosX = {nextPos.x, position.y};
    Vector2 nextPosY = {position.x, nextPos.y};

    if (!IsColliding(nextPos, collision_lines))
    {
        UpdatePosition(nextPos);
    }
    else if (!IsColliding(nextPosX, collision_lines))
    {
        UpdatePosition(nextPosX);
    }
    else if (!IsColliding(nextPosY, collision_lines))
    {
        UpdatePosition(nextPosY);
    }
    else
    {
        return false;
    }

    if (Vector2Equals(position, old_position))
    {
        return false;
    }

    direction = CalculateDirection(CalculateAngle(moveDirection));
    return true;
}

void EntityPosition::UpdatePosition(Vector2 newPosition)
{
    position = newPosition;
    entity_feet = CalculateEntityFeet(newPosition);
    positionRectangle.x = position.x;
    positionRectangle.y = position.y;

    collisionRectangle.x = position.x + (positionRectangle.width * COLLISION_OFFSET_X);
    collisionRectangle.y = position.y + (positionRectangle.height * COLLISION_OFFSET_Y);
    collisionRectangle.width = positionRectangle.width * COLLISION_WIDTH;
    collisionRectangle.height = positionRectangle.height * COLLISION_HEIGHT;
}