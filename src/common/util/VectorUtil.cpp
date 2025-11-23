#include "VectorUtil.h"
#include "raymath.h"

Vector2 IsoWorldToScreen(float x, float y)
{
    return (Vector2){(x - y) * (256 / 2.0f), (x + y) * (128 / 2.0f)};
}

Vector2 ToIso(Vector2 v)
{
    Vector2 out;
    out.x = v.x - v.y;
    out.y = (v.x + v.y) * 0.5f;
    return out;
}

Vector2 NormalizeVectorInIso(Vector2 &v)
{
    float len = sqrtf(v.x * v.x + v.y * v.y);
    v.x /= len;
    v.y /= len;

    return v;
}