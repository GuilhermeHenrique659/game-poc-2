#include "../common/Subject.h"
#include "raylib.h"

class Enemy : public Subject
{
private:
public:
    Vector2 position;

    Enemy(Vector2 position);
    ~Enemy();

    void follow(Vector2 position);
};
