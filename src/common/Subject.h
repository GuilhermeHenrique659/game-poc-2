#pragma once

#include <map>
#include <string>
#include <any>
#include <memory>
#include <utility>

class Observer
{
public:
    Observer() = default;
    virtual ~Observer() = default;

    virtual void notify(const std::any &data) = 0;
};

class Subject
{
private:
    std::map<std::string, std::unique_ptr<Observer>> observers = std::map<std::string, std::unique_ptr<Observer>>();

protected:
    void _publish(const std::string &event, const std::any &data);

public:
    Subject() = default;
    virtual ~Subject() = default;

    void subscribe(const std::string &event, std::unique_ptr<Observer> observer);
};