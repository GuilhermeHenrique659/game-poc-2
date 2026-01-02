#pragma once

#include <memory>
#include <cstdint>
#include <unordered_map>
#include "SpriteAnimation.h"
#include "../entity/common/Entity.h"

class ViewManager
{
private:
    std::unordered_map<uint32_t, std::shared_ptr<SpriteAnimation>> entity_views;

public:
    ViewManager() = default;
    ~ViewManager() = default;

    void CreateView(std::shared_ptr<Entity> entity);
    std::shared_ptr<SpriteAnimation> GetView(uint32_t entity_id);
};
