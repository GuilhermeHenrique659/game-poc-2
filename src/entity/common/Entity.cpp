#include "Entity.h"

Direction Entity::GetEntityDirection()
{
    return entityPosition->GetDirection();
}

void Entity::SetDirection(Direction dir)
{
    entityPosition->SetDirection(dir);
}

void Entity::UpdatePosition(Vector2 position)
{
    entityPosition->UpdatePosition(position);
}

Rectangle Entity::GetDestReactangle()
{
    return entityPosition->GetPositionRectangle();
}

Vector2 Entity::GetPosition()
{
    return entityPosition->GetPostion();
}

Rectangle &Entity::GetCollisionRectangle()
{
    return entityPosition->GetCollisionRectangle();
}