#pragma once
#include "../../common/Subject.h"
#include "raylib.h"
#include "PlayerSpriteAnimation.h"
#include "PlayerState.h"
#include <vector>
#include <optional>
#include "../common/EntityPosition.h"

class Player : public Subject
{
private:
    PlayerState playerState = PlayerState::IDLE;
    EntityPosition *playerPosition;

    PlayerSpriteAnimation playerSpriteAnimation;
    std::optional<Rectangle> attackHitbox;

    float attackTimer = 0.0f;
    float attackCooldown = 0.8f;
    float hitTimer = 0.0f;
    const float HIT_COOLDOWN = 1.28f;
    bool canTakeDamage = true;

    Vector2 NormalizeMove(Vector2 &moveDir);

public:
    int health = 10;

    Player(Vector2 position, Rectangle destRec, PlayerSpriteAnimation playerSpriteAnimation) : playerSpriteAnimation(playerSpriteAnimation),
                                                                                               playerPosition(new EntityPosition(position, Direction::DOWN, destRec, {position.x, position.y, destRec.width / 4, destRec.height / 4}, 4.0f)) {};
    ~Player() = default;

    Rectangle GetDestReactangle();
    Vector2 GetPosition();
    PlayerSpriteAnimation GetPlayerSprite();

    PlayerState GetPlayerState();
    void SetPlayerState(PlayerState state);

    PlayerDirection GetPlayerDirection();
    void SetPlayerDirection(PlayerDirection playerDirection);
    void UpdatePosition(Vector2 newPosition);

    void move(Camera2D camera, std::vector<Rectangle> collisionRectangles);

    void Animate();
    void Attack();

    bool IsHitted(Rectangle rect);
    void OnHit(Rectangle rect);

    void CreateAttackHitbox();
    std::optional<Rectangle> GetAttackHitbox();

    Rectangle &GetCollisionRectangle();
};
