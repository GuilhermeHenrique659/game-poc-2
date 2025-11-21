#include "Hover.h"
#include "button.h"

void OnHover::notify(const std::any &data)
{
    Button *button = std::any_cast<Button *>(data);

    button->setColor(GRAY);
}