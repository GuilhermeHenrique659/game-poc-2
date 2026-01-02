#pragma once;

#include "raylib.h"
#include "Direction.h"
#include <vector>
#include "../../common/CollisionLines.h"

class EntityPosition
{
private:
    Vector2 position;
    Direction direction;
    Rectangle positionRectangle;
    Rectangle collisionRectangle;
    Vector2 entity_feet;
    const float speed;

    Direction CalculateDirection(float angle);
    float CalculateAngle(Vector2 &moveDir);
    Vector2 NormalizeMove(Vector2 &moveDir);
    Vector2 CalculateEntityFeet(Vector2 new_position);
    bool IsColliding(Vector2 position, std::vector<CollisionLines> collision_lines);

public:
    EntityPosition(Vector2 position, Direction direction, Rectangle rectangle, Rectangle collisionRectangle, const float speed) : position(position),
                                                                                                                                  direction(direction),
                                                                                                                                  positionRectangle(rectangle),
                                                                                                                                  collisionRectangle(collisionRectangle),
                                                                                                                                  speed(speed) {};
    ~EntityPosition() = default;

    Vector2 GetEntityFeet();
    Vector2 GetPostion() const { return position; };
    Direction GetDirection() const { return direction; };
    Rectangle GetPositionRectangle() const { return positionRectangle; };
    Rectangle &GetCollisionRectangle() { return collisionRectangle; };
    void SetDirection(Direction newDirection) { direction = newDirection; };

    void UpdatePosition(Vector2 newPosition);
    bool MoveAndCollision(Vector2 moveDirection, std::vector<CollisionLines> collision_lines);
};
