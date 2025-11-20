#pragma once // <--- ESSA LINHA SALVA VIDAS

#include "map"
#include "string"
#include "any"
#include "memory"

class Observer
{
public:
    Observer() = default; // ADICIONE ISSO
    virtual ~Observer() = default;

    virtual void notify(const std::any &data) = 0;
};

class Entity
{
private:
    std::map<std::string, std::unique_ptr<Observer>> observers;

protected:
    void update(const std::string &event, const std::any &data);

public:
    Entity() = default;
    virtual ~Entity() = default;

    void subscribe(const std::string &event, std::unique_ptr<Observer> observer);
};