#pragma once
#include "../common/Entity.h"
#include "../common/EntityPosition.h"

class Player : public Entity
{
public:
    Player(uint32_t id, std::string label, std::unique_ptr<EntityPosition> entityPosition) : Entity(label, id, std::move(entityPosition)) {};
    ~Player() = default;
};