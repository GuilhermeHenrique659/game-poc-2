#pragma once;

#include "../../common/Subject.h"
#include "./EntityPosition.h"
#include "./Direction.h"
#include "raylib.h"
#include <memory>
#include <cstdint>
#include <string>

class Entity : public Subject
{
private:
protected:
    std::unique_ptr<EntityPosition> entityPosition;

public:
    uint32_t id;
    std::string label;

    Entity(std::string label, uint32_t id, std::unique_ptr<EntityPosition> entityPosition) : id(id), label(label), entityPosition(std::move(entityPosition)) {};
    ~Entity() = default;

    Direction GetEntityDirection();
    void SetDirection(Direction direction);

    void UpdatePosition(Vector2 position);

    Rectangle &GetCollisionRectangle();

    Rectangle GetDestReactangle();
    Vector2 GetPosition();
    Vector2 GetEntityFeet();
};
