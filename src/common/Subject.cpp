#include "map"
#include <any>
#include <string>
#include <memory>
#include "Subject.h" // inclui o .h (onde Observer já está definido)

using namespace std;

void Subject::update(const string &event, const any &data)
{
    auto it = observers.find(event);
    if (it != observers.end())
    {
        it->second->notify(data);
    }
}

void Subject::subscribe(const std::string &event, std::unique_ptr<Observer> observer)
{
    observers[event] = move(observer);
}