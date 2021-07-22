#ifndef SIMULATIONWORLD_FOOD_H
#define SIMULATIONWORLD_FOOD_H

#include "objects_hitboxes/screen_object.h"
#include "objects_hitboxes/rectangle.h"
#include "simulation_constants.h"

class Food : public ScreenObject
{
public:
    int energy; // Energy that gives that food instance.
    bool hasBeenEaten = false;

    Food() : ScreenObject() // Constructor with no arguments.
    {
        this->energy = 0.0;
    }
    Food(int ID, int x, int y, int energy) : ScreenObject(ID, x, y, FOOD_SCREEN_SIZE, FOOD_SCREEN_SIZE)// Constructor.
    {
        this->energy = energy;
    }
};

#endif //SIMULATIONWORLD_FOOD_H
