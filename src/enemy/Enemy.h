#include "../common/Entity.h"
#include "raylib.h"

class Enemy : public Entity
{
private:
public:
    Vector2 position;

    Enemy(Vector2 position);
    ~Enemy();

    void follow(Vector2 position);
};
