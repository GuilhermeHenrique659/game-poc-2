#include "EntityAnimationManager.h"

void OnStateChangeListener::notify(const std::any &data)
{
    animationSprite->Reset();
}