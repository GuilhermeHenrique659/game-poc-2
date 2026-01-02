// EventDefinitions.h
#pragma once
#include <string>
#include <unordered_map>

enum EventName
{
    CONNECTION,
    REMOTE_INPUT,
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
    {1, "remote_input"},
    {2, "id_assign"},
    {3, "world_snapshot"},
};