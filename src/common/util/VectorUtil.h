#include "raylib.h"

Vector2 IsoWorldToScreen(float x, float y, float w, float h);

Vector2 ToIso(Vector2 v);

Vector2 NormalizeVectorInIso(Vector2 &v);

Vector2 GetCenter(Rectangle referenceRectangle, Vector2 toVector);