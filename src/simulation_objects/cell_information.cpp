#include "cell_information.h"

CellInformation::CellInformation()
{
    this->x = 0; this->y = 0;
    this->isEmpty = true;
}

CellInformation::CellInformation(int x, int y, bool isEmpty)
{
    this->x = x; this->y = y;
    this->isEmpty = isEmpty;
}

CellInformation::CellInformation(int x, int y, bool isEmpty, Food *foodOnIt)
{
    this->x = x; this->y = y;
    this->isEmpty = isEmpty;
    this->foodOnIt = foodOnIt;
}

void CellInformation::clear()
{
    this->isEmpty = true;
    this->foodOnIt = NULL;
    this->creatureOnIt = NULL;
}

void CellInformation::setCreature(Creature *toSet)
{
    this->creatureOnIt = toSet;
    this->isEmpty = false;
}

void CellInformation::setFood(Food *toSet)
{
    this->foodOnIt = toSet;
    this->isEmpty = false;
}

bool CellInformation::thereIsCreature()
{
    return (this->creatureOnIt != NULL);
}

bool CellInformation::thereIsFood()
{
    return (this->foodOnIt != NULL);
}

void CellInformation::removeCreature()
{
    this->creatureOnIt = NULL;
    this->isEmpty = !(this->thereIsFood());
}

void CellInformation::removeFood()
{
    this->foodOnIt = NULL;
    this->isEmpty = !(this->thereIsCreature());
}
