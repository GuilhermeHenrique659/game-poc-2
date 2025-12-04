#pragma once

#include "../../common/Subject.h"
#include "raylib.h"
#include "PlayerState.h"
#include <vector>
#include <optional>
#include "../common/EntityPosition.h"
#include "../common/EntityAnimationSprite.cpp"
#include "../common/EntityAttack.h"
#include "../common/Entity.h"

class Player : public Entity
{
private:
    PlayerState playerState = PlayerState::IDLE;
    EntityPosition *playerPosition;

    EntityAnimationSprite playerSpriteAnimation;
    EntityAttack *basicAttack;

    float hitTimer = 0.0f;
    const float HIT_COOLDOWN = 1.28f;
    bool canTakeDamage = true;

    Vector2 NormalizeMove(Vector2 &moveDir);

public:
    int health = 10;

    Player(uint32_t id, Vector2 position, Rectangle destRec) : Entity("Player", id),
                                                               playerSpriteAnimation(EntityAnimationSprite()),
                                                               playerPosition(new EntityPosition(position, Direction::DOWN, destRec, {position.x, position.y, destRec.width / 4, destRec.height / 4}, 4.0f)),
                                                               basicAttack(new EntityAttack(destRec.width * 0.5f, destRec.height * 0.5f, 60.0f, 0.8f, 1.28f)) {};
    ~Player() = default;

    Rectangle GetDestReactangle();
    Vector2 GetPosition();
    EntityAnimationSprite GetPlayerSprite();

    PlayerState GetPlayerState();
    void SetPlayerState(PlayerState state);

    Direction GetPlayerDirection();
    void SetPlayerDirection(Direction playerDirection);
    void UpdatePosition(Vector2 newPosition);

    void move(std::vector<Rectangle> collisionRectangles);

    void Animate();
    void Attack();

    bool IsHitted(Rectangle rect);
    void OnHit(Rectangle rect);

    std::optional<Rectangle> GetAttackHitbox();

    Rectangle &GetCollisionRectangle();
};
