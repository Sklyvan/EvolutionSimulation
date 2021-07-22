#ifndef SIMULATIONWORLD_CELL_INFORMATION_H
#define SIMULATIONWORLD_CELL_INFORMATION_H

#include <cstdio>
#include "creature.h"
#include "food.h"

class CellInformation
{
public:
    int x, y; // Cell position on screen.
    bool isEmpty;
    Creature* creatureOnIt = NULL;
    Food* foodOnIt = NULL;

    CellInformation();
    CellInformation(int x, int y, bool isEmpty);
    CellInformation(int x, int y, bool isEmpty, Food* foodOnIt);

    void setCreature(Creature* toSet);
    void setFood(Food* toSet);
    void removeCreature();
    void removeFood();
    bool thereIsCreature();
    bool thereIsFood();
    void clear(); // Deletes everything from it
};

#endif //SIMULATIONWORLD_CELL_INFORMATION_H
