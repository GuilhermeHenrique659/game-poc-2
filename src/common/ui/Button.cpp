#include "Button.h"
#include "raylib.h"

Button::Button(char const *label, int x, int y, int width, int height)
    : label(label), x(x), y(y), width(width), height(height), color(LIGHTGRAY)
{
    defaultColor = color;
}

void Button::Update()
{
    Vector2 mousePoint = GetMousePosition();
    bool isMouseOver = CheckCollisionPointRec(mousePoint, {static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height)});

    if (isMouseOver)
    {
        _update("OnHover", this);
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isMouseOver)
    {
        _update("OnClick", this);
    }

    if (!isMouseOver)
    {
        color = defaultColor;
    }
}

void Button::Draw()
{
    DrawRectangle(x, y, width, height, color);
    DrawText(label.c_str(), x + 10, y + 10, 20, DARKGRAY);
}