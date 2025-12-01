#pragma once
#include "../../common/Subject.h"
#include "raylib.h"
#include "PlayerSpriteAnimation.h"
#include "PlayerState.h"
#include <vector>
#include <optional>

class Player : public Subject
{
private:
    PlayerDirection playerDirection = DOWN;
    PlayerState playerState = IDLE;

    PlayerSpriteAnimation playerSpriteAnimation;
    Vector2 position;
    Rectangle destRec;
    Rectangle collisionRectangle;
    std::optional<Rectangle> attackHitbox;

    float attackTimer = 0.0f;
    float attackDuration = 1.2f;
    float attackCooldown = 0.8f;

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
    void Attack();

    bool IsHitted(Rectangle rect);

    void CreateAttackHitbox();
    std::optional<Rectangle> GetAttackHitbox();

    void UpdateCollisionRectangle();

    Rectangle &GetCollisionRectangle();

    Rectangle GetFutureCollisionRectangle(Vector2 futurePosition) const;
};
