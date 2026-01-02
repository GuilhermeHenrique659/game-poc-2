#include "EntityManager.h"

uint32_t EntityManager::GetEntityId()
{
    uint32_t next_id = entities.size() + 1;

    return next_id;
}

std::unordered_map<std::uint32_t, std::shared_ptr<Entity>> &EntityManager::GetEntities()
{
    return entities;
}

void EntityManager::AddEntity(std::shared_ptr<Entity> entity)
{
    TraceLog(LOG_INFO, "Add entity id(%d)", entity->id);
    entities[entity->id] = entity;
    TraceLog(LOG_INFO, "Number of entities %d", entities.size());
}

void EntityManager::RemoveEntity(uint32_t entityId)
{
    entities.erase(entityId);
}

std::optional<std::shared_ptr<Entity>> EntityManager::GetEntity(uint32_t entityId)
{
    if (entities.find(entityId) != entities.end())
    {
        return std::optional<std::shared_ptr<Entity>>(entities[entityId]);
    }

    return std::optional<std::shared_ptr<Entity>>();
}