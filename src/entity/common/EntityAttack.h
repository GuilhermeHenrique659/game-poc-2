#pragma once

#include <raylib.h>
#include <optional>
#include "EntityPosition.h"

class EntityAttack
{
private:
    float attackboxWidth;
    float attackboxHeight;
    float attackboxlenght;

    float attackTimer = 0.0f;
    float attackCooldown;
    float attackDuration;

    std::optional<Rectangle> attackbox;

    void createAttackbox(EntityPosition *entityPosition);

public:
    EntityAttack(float attackboxWidth, float attackboxHeight, float attackboxlenght, float attackCooldown, float attackDuration) : attackboxHeight(attackboxHeight),
                                                                                                                                   attackboxWidth(attackboxWidth),
                                                                                                                                   attackboxlenght(attackboxlenght),
                                                                                                                                   attackCooldown(attackCooldown),
                                                                                                                                   attackDuration(attackDuration) {};
    ~EntityAttack() = default;

    bool attack(EntityPosition *entityPosition);
    std::optional<Rectangle> getAttackbox() { return attackbox; }
};