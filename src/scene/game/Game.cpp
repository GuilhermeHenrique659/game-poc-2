#include "Game.h"
#include "../../entity/player/Player.h"

void Game::Setup()
{
    entity_manager = std::make_unique<EntityManager>();
    view_manager = std::make_unique<ViewManager>();

    auto player = std::make_shared<Player>(entity_manager->GetEntityId(), "Player", std::make_unique<EntityPosition>(Vector2{100.0f, 100.0f}, Direction::DOWN, Rectangle{100.0f, 100.0f, 320.0f, 320.0f}, Rectangle{100.0f, 100.0f, 320.0f, 320.0f}, 200.0f));

    entity_manager->AddEntity(player);
    view_manager->CreateView(player);
}

void Game::Update(float delta)
{
}

void Game::Presenter(float delta)
{
    for (auto &[id, entity] : entity_manager->GetEntities())
    {
        auto view = view_manager->GetView(id);
        if (view)
        {
            static SpriteCollection sprite_collection;
            sprite_collection.AddSprite(Direction::DOWN, {.texture_key = "idle_stand_down",
                                                          .frame_width = 320,
                                                          .frame_height = 320,
                                                          .total_frames = 16,
                                                          .columns = 4,
                                                          .rows = 4});

            view->Animate(sprite_collection, entity->GetEntityDirection());

            auto texture = view->GetTexture();
            auto source_rect = view->GetSourceRectangle();

            DrawTexturePro(
                texture,
                source_rect,
                entity->GetDestReactangle(),
                {0, 0},
                0,
                Fade(WHITE, 0.8f));
        }
    }
}
