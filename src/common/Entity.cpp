#include "map"
#include <any>
#include <string>
#include <memory>
#include "Entity.h" // inclui o .h (onde Observer já está definido)

using namespace std;

void Entity::update(const string &event, const any &data)
{
    auto it = observers.find(event);
    if (it != observers.end())
    {
        it->second->notify(data);
    }
}

void Entity::subscribe(const std::string &event, std::unique_ptr<Observer> observer)
{
    observers[event] = move(observer);
}