#include "Subject.h"

void Subject::_update(const std::string &event, const std::any &data)
{
    auto it = observers.find(event);
    if (it != observers.end())
    {
        it->second->notify(data);
    }
}

void Subject::subscribe(const std::string &event, std::unique_ptr<Observer> observer)
{
    observers.emplace(event, std::move(observer));
}