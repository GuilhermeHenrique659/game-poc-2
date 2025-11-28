#include "Subject.h"
#include "raylib.h"

void Subject::_publish(const std::string &event, const std::any &data)
{
    /*     TraceLog(LOG_INFO, "Subject: Notifying observers about event '%s'.", event.c_str());
        TraceLog(LOG_INFO, "Subject: Notifying observers '%d'.", observers.size());
     */
    if (observers.empty())
        return;

    auto it = observers.find(event);
    if (it == observers.end())
        return;

    if (it != observers.end())
    {
        it->second->notify(data);
    }
}

void Subject::subscribe(const std::string &event, std::unique_ptr<Observer> observer)
{
    TraceLog(LOG_INFO, "Subject: Notifying observers about event '%s'", event.c_str());
    observers.emplace(event, std::move(observer));
}