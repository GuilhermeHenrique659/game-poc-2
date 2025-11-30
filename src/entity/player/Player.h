#pragma once
#include "../../common/Subject.h"
#include "raylib.h"
#include "PlayerSpriteAnimation.h"

class Player : public Subject
{
private:
    PlayerDirection playerDirection = DOWN;
    PlayerSpriteAnimation playerSpriteAnimation;

    void NormalizeMove(Vector2 &moveDir);

public:
    float angle = 0;
    Vector2 position;
    Rectangle destRec;
    bool isIdle = true;

    Player(Vector2 position, Rectangle destRec, PlayerSpriteAnimation playerSpriteAnimation) : position(position), destRec(destRec), playerSpriteAnimation(playerSpriteAnimation) {};
    ~Player() = default;

    void move(Camera2D camera);
    void CalculateAngle(const Camera2D &camera, Vector2 &moveDir);
    int CalculateDirection();

    PlayerSpriteAnimation GetPlayerSprite();
    PlayerDirection GetPlayerDirection();
    void SetPlayerDirection(PlayerDirection playerDirection);

    void Animate();
};
