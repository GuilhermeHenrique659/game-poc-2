#pragma once

#include <raylib.h>
#include <string>
#include <optional>
#include "EntityPosition.h"

class EntityAttack
{
private:
    std::string attack_name;
    std::string attack_type;

    float attackboxWidth;
    float attackboxHeight;
    float attackboxlenght;

    float attackTimer = 0.0f;
    int attack_points_consume;
    float attackDuration;

    std::optional<Rectangle> attackbox;

    void createAttackbox(EntityPosition *entityPosition);

public:
    EntityAttack(std::string attack_name, std::string attack_type, float attackboxWidth, float attackboxHeight, float attackboxlenght, int attackPointsConsume, float attackDuration) : attackboxHeight(attackboxHeight),
                                                                                                                                   attackboxWidth(attackboxWidth),
                                                                                                                                   attackboxlenght(attackboxlenght),
                                                                                                                                   attack_points_consume(attackPointsConsume),
                                                                                                                                   attackDuration(attackDuration),
                                                                                                                                   attack_name(attack_name),
                                                                                                                                   attack_type(attack_type) {};
    ~EntityAttack() = default;

    const std::string& GetAttackName() const;
    const std::string& GetAttackType() const;

    bool attack(EntityPosition *entityPosition);
    std::optional<Rectangle> getAttackbox() { return attackbox; }
    float GetAttackDuration() const { return attackDuration; }
    int GetAttackPointsConsume() const { return attack_points_consume; }
};