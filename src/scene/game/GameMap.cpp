#include "GameMap.h"
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

#include "../../common/util/VectorUtil.h"
#include "../../common/ResourceManager.h"

tmx::Tileset FindTileByGID(int gid, std::vector<tmx::Tileset> tile_sets)
{
    for (const auto &tileset : tile_sets)
    {
        if (gid >= tileset.getFirstGID() && gid <= tileset.getLastGID())
        {
            return tileset;
        }
    }

    TraceLog(LOG_WARNING, "Tile GID %u sem tileset correspondente!", gid);
}

void GameMap::Init()
{
    auto &resource_manager = ResourceManager::Get();
    auto &map = resource_manager.GetMap();

    const auto &layers = map.getLayers();
    const auto &tile_sets = map.getTilesets();

    for (const auto &layer : layers)
    {
        if (layer->getType() == tmx::Layer::Type::Tile)
        {
            const auto &tile_layer = layer->getLayerAs<tmx::TileLayer>();
            const auto &tiles = tile_layer.getTiles();

            auto map_size = map.getTileCount();
            auto tile_size = map.getTileSize();

            for (unsigned int y = 0; y < map_size.y; ++y)
            {
                for (unsigned int x = 0; x < map_size.x; ++x)
                {
                    auto idx = y * map_size.x + x;
                    auto gid = tiles[idx].ID;

                    if (gid == 0)
                        continue;

                    unsigned int cleanGid = gid & 0x0FFFFFFF;
                    auto tile_set = FindTileByGID(cleanGid, tile_sets);

                    Texture2D texture = resource_manager.GetTexture(tile_set.getName());

                    Vector2 screenPos = IsoWorldToScreen((float)x, (float)y, (float)tile_size.x, (float)tile_size.y);

                    float offsetX = GetScreenWidth() / 2.0f;
                    screenPos.x += offsetX;
                    unsigned int localTileId = cleanGid - tile_set.getFirstGID();

                    float tileWidth = (float)tile_set.getTileSize().x;
                    float tileHeight = (float)tile_set.getTileSize().y;

                    unsigned int imageWidth = tile_set.getImageSize().x;
                    unsigned int imageHeight = tile_set.getImageSize().y;

                    unsigned int realColumns = imageWidth / tileWidth;

                    unsigned int tileX = (localTileId % realColumns) * tileWidth;
                    unsigned int tileY = (localTileId / realColumns) * tileHeight;

                    Rectangle sourceRect = {
                        (float)tileX,
                        (float)tileY,
                        (float)tileWidth,
                        (float)tileHeight};

                    Rectangle tileRectangle = {screenPos.x, screenPos.y, (float)texture.width, (float)texture.height};
                    auto sprite = StaticSprite{
                        texture,
                        screenPos,
                        sourceRect,
                        tileRectangle};

                    map_sprites[layer->getName()].push_back(sprite);
                }
            }
        }
    }
}

void GameMap::Draw(const std::string &layer_name)
{
    for (auto &sprite : map_sprites[layer_name])
    {
        DrawTexturePro(sprite.texture, sprite.source_rectangle, sprite.dest_rectangle, {0, 0}, 0, WHITE);
    }
}

std::vector<Rectangle> GameMap::GetCollisionTiles()
{
    return collisition_tiles;
}