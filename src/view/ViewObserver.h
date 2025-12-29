#pragma once
#include "../common/Subject.h"
#include <memory>
#include "SpriteAnimation.h"

class ViewObserver : public Observer
{
private:
    std::shared_ptr<SpriteAnimation> sprite_animation;

public:
    ViewObserver(std::shared_ptr<SpriteAnimation> sprite_animation) : sprite_animation(sprite_animation) {};
    ~ViewObserver() = default;

    void notify(const std::any &data) override;
};
