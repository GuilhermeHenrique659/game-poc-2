#include "EntityAttack.h"
#include "../../config.h"

void EntityAttack::createAttackbox(EntityPosition *entityPosition)
{
    auto position = entityPosition->GetPostion();
    auto direction = entityPosition->GetDirection();
    auto destRec = entityPosition->GetPositionRectangle();

    Rectangle rightAttackbox = {position.x + destRec.width * 0.64f, position.y, attackboxlenght, attackboxHeight};

    Rectangle leftAttackbox = {position.x + 72.0f, position.y, attackboxlenght, attackboxHeight};

    Rectangle upAttackbox = {position.x, position.y + 40.0f, attackboxWidth, attackboxlenght};

    Rectangle downAttackbox = {position.x, position.y + destRec.height * 0.64f, attackboxWidth, attackboxlenght};

    switch (direction)
    {
    case Direction::RIGHT:
        attackbox = rightAttackbox;
        break;
    case Direction::RIGHT_DOWN:
        attackbox = rightAttackbox;
        break;
    case Direction::RIGHT_UP:
        attackbox = upAttackbox;
        break;
    case Direction::LEFT:
        attackbox = leftAttackbox;
        break;
    case Direction::LEFT_DOWN:
        attackbox = leftAttackbox;
        break;
    case Direction::LEFT_UP:
        attackbox = upAttackbox;
        break;
    case Direction::UP:
        attackbox = upAttackbox;
        break;
    case Direction::DOWN:
        attackbox = downAttackbox;
        break;
    default:
        attackbox = downAttackbox;
    }
}

bool EntityAttack::attack(EntityPosition *entityPosition)
{
    if (attackTimer < attackDuration && attackTimer > 0)
    {
        attackTimer += GetFrameTime();

        return false;
    }

    if (attackTimer >= attackDuration)
    {
        attackTimer = 0.0f;

        return true;
    }

    attackTimer = 0.0f;

    attackTimer += GetFrameTime();

    createAttackbox(entityPosition);

    return true;
}