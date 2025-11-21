#pragma once

class Scene
{
public:
    Scene() = default;
    virtual ~Scene() = default;

    virtual void Setup() = 0;
    virtual void Presenter(float delta) = 0;
    virtual void Update(float delta) = 0;
};