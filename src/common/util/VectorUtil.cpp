#include "VectorUtil.h"
#include "raymath.h"

Vector2 IsoWorldToScreen(float x, float y, float w, float h)
{
    return (Vector2){(x - y) * (w / 2.0f), (x + y) * (h / 2.0f)};
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

Vector2 GetCenter(Rectangle referenceRectangle, Vector2 toCenter)
{
    return Vector2{
        referenceRectangle.x + referenceRectangle.width / 2 - (toCenter.x / 2),
        referenceRectangle.y + referenceRectangle.height / 2 - (toCenter.y / 2)};
}