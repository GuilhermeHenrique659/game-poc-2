#pragma once

#include <unordered_map>
#include <cstdint>
#include <memory>
#include <optional>
#include "common/Entity.h"

class EntityManager
{
private:
    std::unordered_map<std::uint32_t, std::shared_ptr<Entity>> entities;

public:
    EntityManager() = default;
    ~EntityManager() = default;

    void AddEntity(std::shared_ptr<Entity> entity);
    void RemoveEntity(uint32_t entity_id);

    uint32_t GetEntityId();

    template <typename T>
    std::optional<std::shared_ptr<T>> GetEntity(uint32_t entity_id);

    std::unordered_map<std::uint32_t, std::shared_ptr<Entity>> &GetEntities();
};
