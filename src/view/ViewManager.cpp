#include "ViewManager.h"
#include "ViewObserver.h"

void ViewManager::CreateView(std::shared_ptr<Entity> entity)
{
    auto sprite_animation = std::make_shared<SpriteAnimation>();

    auto view_observer = std::make_unique<ViewObserver>(sprite_animation);
    entity->subscribe("position_changed", std::move(view_observer));

    entity_views[entity->id] = sprite_animation;
}

std::shared_ptr<SpriteAnimation> ViewManager::GetView(uint32_t entity_id)
{
    auto it = entity_views.find(entity_id);
    if (it != entity_views.end())
    {
        return it->second;
    }
    return nullptr;
}