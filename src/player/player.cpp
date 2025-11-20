
#include "stdio.h"
#include "raylib.h"
#include "../common/Entity.h"
#include <raymath.h>
#include "any"

using namespace std;

class Player : public Entity
{
private:
    /* data */
public:
    Vector2 position;
    Player(Vector2 position)
    {
        this->position = position;
    }

    void move()
    {
        Vector2 olpPosition = position;
        if (IsKeyDown(KEY_RIGHT))
            position.x += 2.0f;
        if (IsKeyDown(KEY_LEFT))
            position.x -= 2.0f;
        if (IsKeyDown(KEY_UP))
            position.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN))
            position.y += 2.0f;

        if (!Vector2Equals(olpPosition, position))
            update("moved", std::any(position));
    }
};