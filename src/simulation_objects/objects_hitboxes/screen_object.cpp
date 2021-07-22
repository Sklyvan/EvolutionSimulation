#include "screen_object.h"

ScreenObject::ScreenObject()
{
    this->ID = -1;
    this->x = 0; this->y = 0;
    this->w = 0; this->h = 0;
    this->isVisible = false;
    this->HitBox = Rectangle();
}

ScreenObject::ScreenObject(int ID, int x, int y, int w, int h)
{
    this->ID = ID;
    this->x = x; this->y = y;
    this->w = w; this->h = h;
    this->isVisible = true;
    this->HitBox = Rectangle(x, y, w, h);
}

ScreenObject::ScreenObject(int ID, int x, int y, int w, int h, bool isVisible)
{
    this->ID = ID;
    this->x = x; this->y = y;
    this->w = w; this->h = h;
    this->isVisible = isVisible;
    this->HitBox = Rectangle(x, y, w, h);
}

void ScreenObject::updateHitBox()
{
    this->HitBox.x = this->x; this->HitBox.y = this->y;
    this->HitBox.width = this->w; this->HitBox.height = this->h;
}

bool ScreenObject::isTouching(ScreenObject withObject)
{
    return this->HitBox.isColliding(withObject.HitBox);
}

bool ScreenObject::hasInside(ScreenObject withObject)
{
    return this->HitBox.isContaining(withObject.HitBox);
}