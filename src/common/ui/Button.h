#pragma once

#include "../Subject.h"
#include "raylib.h"

class Button : public Subject
{
private:
    Color color;
    int x;
    int y;
    int width;
    int height;
    Color defaultColor;

public:
    std::string label;

    Button(char const *label, int x, int y, int width, int height);
    ~Button() = default;

    Button(Button &&) = default;
    Button &operator=(Button &&) = default;

    Button(const Button &) = delete;
    Button &operator=(const Button &) = delete;

    void Update();
    void Draw();

    void setColor(Color newColor) { color = newColor; }
};
