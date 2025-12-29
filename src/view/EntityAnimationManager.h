#pragma once

#include <unordered_map>
#include "../entity/common/Entity.h"
#include <memory>
#include "../common/Subject.h"
#include "EntityAnimationSprite.cpp"

class OnStateChangeListener : public Observer
{
private:
    std::shared_ptr<EntityAnimationSprite> animationSprite;

public:
    OnStateChangeListener(std::shared_ptr<EntityAnimationSprite> animationSprite) : animationSprite(animationSprite) {};
    ~OnStateChangeListener() = default;

    void notify(const std::any &data) override;
};

class EntityAnimationManager
{
private:
    std::unordered_map<uint32_t, std::shared_ptr<EntityAnimationSprite>> entityAnimationSprite;

public:
    EntityAnimationManager() = default;
    ~EntityAnimationManager() = default;

    void AddSprite(std::shared_ptr<Entity> entity)
    {
        entityAnimationSprite[entity->id] = std::make_shared<EntityAnimationSprite>();
        entity->subscribe("state_changed", std::make_unique<OnStateChangeListener>(entityAnimationSprite[entity->id]));
    }

    void Animate(uint32_t entityId, Direction dir, EntitySpriteCollection collection)
    {
        entityAnimationSprite[entityId]->Animate(dir, collection);
    }

    std::shared_ptr<EntityAnimationSprite> GetAnimation(uint32_t entityId)
    {
        return entityAnimationSprite[entityId];
    }
};