#include "../common/Subject.h"
#include "raylib.h"

class Player : public Subject
{
private:
    void NormalizeMove(Vector2 &moveDir);

public:
    float angle = 0;
    Vector2 position;
    Texture2D texture;
    Rectangle sourceRec;
    Rectangle destRec;
    bool isIdle = true;
    int currentFrame = 0;
    float runningTime = 0.0f;

    Player(Vector2 position, Texture2D texture, Rectangle sourceRec, Rectangle destRec) : position(position), texture(texture), sourceRec(sourceRec), destRec(destRec) {};
    ~Player() = default;

    void move(Camera2D ca);
};
