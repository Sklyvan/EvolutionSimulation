#ifndef SIMULATIONWORLD_SCREEN_OBJECT_H
#define SIMULATIONWORLD_SCREEN_OBJECT_H

#include "rectangle.h"

class ScreenObject
{
public:
    int ID;
    int x; int y;
    int w; int h;
    bool isVisible;

    Rectangle HitBox;

    ScreenObject();
    ScreenObject(int ID, int x, int y, int w, int h);
    ScreenObject(int ID, int x, int y, int w, int h, bool isVisible);

    void updateHitBox();
    bool isTouching(ScreenObject withObject);
    bool hasInside(ScreenObject withObject);
};

#endif //SIMULATIONWORLD_SCREEN_OBJECT_H
