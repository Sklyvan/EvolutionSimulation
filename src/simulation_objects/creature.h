#ifndef SIMULATIONWORLD_CREATURE_H
#define SIMULATIONWORLD_CREATURE_H

#include "food.h"
#include "objects_hitboxes/rectangle.h"
#include "objects_hitboxes/screen_object.h"
#include "simulation_constants.h"
#include <list>

class Creature : public ScreenObject
{
public:
    float speed, secondUnitProb, energy, currentEnergy;
    int eatenFoods;
    Creature* createdBy;
    std::list<Creature*> childs;
    int direction, holdDirectionCounter;
    int changeDirectionCoef;
    bool isAlive, isAtHome, isGoingHome;
    int homeLocation[2];

    Creature() : ScreenObject() // Empty constructor.
    {
        this->speed = 0.0;
        this->secondUnitProb = 0.0;
        this->energy = 0.0; this->currentEnergy = 0.0;
        this->eatenFoods = 0;
        this->createdBy = NULL;
        this->direction = -1;
        this->holdDirectionCounter = -1;
        this->isAlive = true;
        this->isAtHome = true;
        this->isGoingHome = false;
        this->homeLocation[0] = -1; this->homeLocation[1] = -1;
        this->changeDirectionCoef = 0;
    }

    Creature(int ID, int x, int y, float speed, float secondUnitProb, float energy, int direction, int chngCoef) : ScreenObject(ID, x, y, CREATURE_SCREEN_SIZE, CREATURE_SCREEN_SIZE)
    {
        this->speed = speed;
        this->secondUnitProb = secondUnitProb;
        this->energy = energy; this->currentEnergy = energy;
        this->eatenFoods = 0;
        this->createdBy = NULL;
        this->direction = direction;
        this->holdDirectionCounter = HOLD_DIRECTION;
        this->isAlive = true;
        this->isAtHome = true;
        this->isGoingHome = false;
        this->homeLocation[0] = -1; this->homeLocation[1] = -1;
        this->changeDirectionCoef = 5;
    }

    Creature(int ID, int x, int y, float speed, float secondUnitProb, float energy, int direction, int chngCoef, Creature* createdBy): ScreenObject(ID, x, y, CREATURE_SCREEN_SIZE, CREATURE_SCREEN_SIZE)
    {
        this->speed = speed;
        this->secondUnitProb = secondUnitProb;
        this->energy = energy; this->currentEnergy = energy;
        this->eatenFoods = 0;
        this->createdBy = createdBy;
        this->direction = direction;
        this->holdDirectionCounter = HOLD_DIRECTION;
        this->isAlive = true;
        this->isAtHome = true;
        this->isGoingHome = false;
        this->homeLocation[0] = -1; this->homeLocation[1] = -1;
        this->changeDirectionCoef = 5;
    }

    void move();
    void getNextDirection(int width, int height); // Este método debe tener en cuenta que la siguiente dirección sea posible y que lo más probable sea seguir recto.

    bool isUPAvaliable(int width, int height);
    bool isDOWNAvaliable(int width, int height);
    bool isLEFTAvaliable(int width, int height);
    bool isRIGHTAvaliable(int width, int height);
    bool canContinue(int width, int height);
    void invertDirection();

    Food* isTouchingAnyFood(Food* foodsList, int listSize);

    void kill();
    void eat(Food toEat);

    void setHomeLocation(int mapWidth, int mapHeight);
    void goHome(int mapWidth, int mapHeight);

    Creature* reproduce(int childID);
    void addChild(Creature* newChild);
    int getNumberOfChilds();
};

#endif //SIMULATIONWORLD_CREATURE_H
