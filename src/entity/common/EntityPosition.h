#pragma once;

#include "raylib.h"
#include "Direction.h"
#include <vector>

class EntityPosition
{
private:
    Vector2 position;
    Direction direction;
    Rectangle positionRectangle;
    Rectangle collisionRectangle;
    const float speed;

    Direction CalculateDirection(float angle);
    float CalculateAngle(Vector2 &moveDir);
    Rectangle GetFutureCollisionRectangle(Vector2 futurePosition) const;
    Vector2 NormalizeMove(Vector2 &moveDir);

public:
    EntityPosition(Vector2 position, Direction direction, Rectangle rectangle, Rectangle collisionRectangle, const float speed) : position(position),
                                                                                                                                  direction(direction),
                                                                                                                                  positionRectangle(rectangle),
                                                                                                                                  collisionRectangle(collisionRectangle),
                                                                                                                                  speed(speed) {};
    ~EntityPosition() = default;

    Vector2 GetPostion() const { return position; };
    Direction GetDirection() const { return direction; };
    Rectangle GetPositionRectangle() const { return positionRectangle; };
    Rectangle &GetCollisionRectangle() { return collisionRectangle; };
    void SetDirection(Direction newDirection) { direction = newDirection; };

    void UpdatePosition(Vector2 newPosition);
    bool MoveAndCollision(Vector2 moveDirection, std::vector<Rectangle> collisionRectangles);
};
