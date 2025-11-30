#pragma once
#include "../../common/Subject.h"
#include "raylib.h"
#include "PlayerSpriteAnimation.h"
#include "PlayerState.h"
#include <vector>

class Player : public Subject
{
private:
    PlayerDirection playerDirection = DOWN;
    PlayerState playerState = IDLE;

    PlayerSpriteAnimation playerSpriteAnimation;
    Vector2 position;
    Rectangle destRec;
    Rectangle collisionRectangle;

    Vector2 NormalizeMove(Vector2 &moveDir);

public:
    float angle = 0;

    Player(Vector2 position, Rectangle destRec, PlayerSpriteAnimation playerSpriteAnimation) : position(position), destRec(destRec), playerSpriteAnimation(playerSpriteAnimation),
                                                                                               collisionRectangle{position.x, position.y, destRec.width / 4, destRec.height / 4} {};
    ~Player() = default;

    Rectangle GetDestReactangle();
    Vector2 GetPosition();
    PlayerSpriteAnimation GetPlayerSprite();

    PlayerState GetPlayerState();
    void SetPlayerState(PlayerState state);

    PlayerDirection GetPlayerDirection();
    void SetPlayerDirection(PlayerDirection playerDirection);

    void UpdatePosition(Vector2 position);

    void move(Camera2D camera, std::vector<Rectangle> collisionRectangles);
    void CalculateAngle(const Camera2D &camera, Vector2 &moveDir);
    int CalculateDirection();

    void Animate();

    void UpdateCollisionRectangle();

    Rectangle &GetCollisionRectangle();

    Rectangle GetFutureCollisionRectangle(Vector2 futurePosition) const;
};
