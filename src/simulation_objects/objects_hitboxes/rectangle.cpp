#include "rectangle.h"

Rectangle::Rectangle()
{
    this->x = 0; this->y = 0;
    this->width = 0; this->height = 0;
}

Rectangle::Rectangle(int x, int y, int w, int h)
{
    this->x = x; this->y = y;
    this->width = w; this->height = h;
}

bool Rectangle::isColliding(Rectangle toTest)
{
    return (this->x < toTest.x + toTest.width && this->x + this->width > toTest.x && this->y < toTest.y + toTest.height && this->y + this->height > toTest.y);
}

bool Rectangle::isContaining(Rectangle toTest)
{
    return ((toTest.x+toTest.width) < (this->x+this->width) && (toTest.x) > (this->x) && (toTest.y) > (this->y) && (toTest.y+toTest.height) < (this->y+this->height));
}