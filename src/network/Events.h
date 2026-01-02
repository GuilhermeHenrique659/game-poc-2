// EventDefinitions.h
#pragma once
#include <string>
#include <unordered_map>

enum EventName
{
    CONNECTION,
    PLAYER_MOVED,
    ID_ASSIGNEED,
    WORLD_SNAPSHOT,
};

struct EventDefinition
{
    int id;
    const char *name;
};

static const EventDefinition EVENT_DEFINITIONS[] = {
    {0, "connection"},
    {1, "player_moved"},
    {2, "id_assign"},
    {3, "world_snapshot"},
};