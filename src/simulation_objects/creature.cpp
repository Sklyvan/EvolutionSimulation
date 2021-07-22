#include <fstream>
#include "creature.h"
#include "image.h"
#include "simulation_constants.h"
#include "Python.h" // For parallel work with Python.

void Creature::move()
{
    if (this->currentEnergy > 0)
    {
        if (this->direction == UP) this->y += this->speed;
        else if (this->direction == DOWN) this->y -= this->speed;
        else if (this->direction == LEFT) this->x -= this->speed;
        else if (this->direction == RIGHT) this->x += this->speed;

        this->currentEnergy -= ENERGY_DECREASE_COEF*this->speed;

        // Check if creature is at home
        if (this->isGoingHome) // If creature is going home, check if now is at home.
        {
            // If creature is at the same location as the home location, then is at home.
            this->isAtHome = (this->homeLocation[0] == this->x && this->homeLocation[1] == this->y);
        }
        else
        {
            this->isAtHome = false;
            this->updateHitBox();
        }
    }
    else
    {
        this->kill();
    }
}

bool Creature::isUPAvaliable(int width, int height)
{
    return (this->y + this->speed + CREATURE_SCREEN_SIZE) < height;
}

bool Creature::isDOWNAvaliable(int width, int height)
{
    return (this->y - this->speed) >= 0;
}

bool Creature::isLEFTAvaliable(int width, int height)
{
    return (this->x - this->speed) >= 0;
}

bool Creature::isRIGHTAvaliable(int width, int height)
{
    return (this->x + this->speed + CREATURE_SCREEN_SIZE) < width;
}

bool Creature::canContinue(int width, int height)
{
    if (this->direction == UP) return this->isUPAvaliable(width, height);
    else if (this->direction == DOWN) return this->isDOWNAvaliable(width, height);
    else if (this->direction == LEFT) return this->isLEFTAvaliable(width, height);
    else if (this->direction == RIGHT) return this->isRIGHTAvaliable(width, height);
    else return false;
}

void Creature::invertDirection()
{
    if (this->direction == UP) this->direction = DOWN;
    else if (this->direction  == DOWN) this->direction = UP;
    else if (this->direction  == LEFT) this->direction = RIGHT;
    else if (this->direction  == RIGHT) this->direction = LEFT;
    else this->direction = -1;
}

void Creature::getNextDirection(int screenWidth, int screenHeight)
{
    if (!this->canContinue(screenWidth, screenHeight)) // If it has to change direction, we invert the direction.
    {
        if (this->direction == UP) this->invertDirection();
        else if (this->direction == DOWN) this->invertDirection();
        else if (this->direction == LEFT) this->invertDirection();
        else if (this->direction == RIGHT) this->invertDirection();

        this->holdDirectionCounter = HOLD_DIRECTION;
    }
    else if (this->holdDirectionCounter == 0)
    {
        bool changeDirection; // La probablidad de cambiar de dirección es de 1/5.
        srand(time(NULL)*this->ID); // Generating pseudo random seeds.
        (rand()%this->changeDirectionCoef == 0) ? changeDirection = true : changeDirection = false;
        if (changeDirection)
        {
            int avaliableDirections[4] = {UP, DOWN, LEFT, RIGHT};
            if (!this->isUPAvaliable(screenWidth, screenHeight)) avaliableDirections[0] = -1;
            if (!this->isDOWNAvaliable(screenWidth, screenHeight)) avaliableDirections[1] = -1;
            if (!this->isLEFTAvaliable(screenWidth, screenHeight)) avaliableDirections[2] = -1;
            if (!this->isRIGHTAvaliable(screenWidth, screenHeight)) avaliableDirections[3] = -1;
            int newDirection = this->direction;
            while(newDirection == this->direction && newDirection != -1) newDirection = avaliableDirections[rand()%4];
            this->direction = newDirection;
            this->holdDirectionCounter = HOLD_DIRECTION;
        }
    }
    else this->holdDirectionCounter--;
}

void Creature::kill()
{
    this->isAlive = false;
    this->isVisible = false;
}

void Creature::eat(Food toEat)
{
    this->currentEnergy += toEat.energy;
    this->eatenFoods++;
}

void Creature::setHomeLocation(int mapWidth, int mapHeight)
{
    int downLocation[2] = {this->x, 0}; int upLocation[2] = {this->x, mapHeight - CREATURE_SCREEN_SIZE - 1};
    int leftLocation[2] = {0, this->y}; int rightLocation[2] = {mapWidth - CREATURE_SCREEN_SIZE - 1, this->y};

    float d1 = distance(downLocation[0], downLocation[1], this->x, this->y); float d2 = distance(upLocation[0], upLocation[1], this->x, this->y);
    float d3 = distance(leftLocation[0], leftLocation[1], this->x, this->y); float d4 = distance(rightLocation[0], rightLocation[1], this->x, this->y);

    if (d1 < d2 && d1 < d3 && d1 < d4)
    {
        this->homeLocation[0] = downLocation[0];
        this->homeLocation[1] = downLocation[1];
        this->direction = DOWN;
    }
    else if (d2 < d1 && d2 < d3 && d2 < d4)
    {
        this->homeLocation[0] = upLocation[0];
        this->homeLocation[1] = upLocation[1];
        this->direction = UP;
    }
    else if (d3 < d1 && d3 < d2 && d3 < d4)
    {
        this->homeLocation[0] = leftLocation[0];
        this->homeLocation[1] = leftLocation[1];
        this->direction = LEFT;
    }
    else if (d4 < d1 && d4 < d2 && d4 < d3)
    {
        this->homeLocation[0] = rightLocation[0];
        this->homeLocation[1] = rightLocation[1];
        this->direction = RIGHT;
    }
    else
    {
        int directions[4] = {UP, DOWN, LEFT, RIGHT}; int directionIndex = rand()%4;
        int randomDirection = directions[directionIndex];
        if (randomDirection == UP)
        {
            this->homeLocation[0] = upLocation[0];
            this->homeLocation[1] = upLocation[1];
            this->direction = UP;
        }
        else if (randomDirection == DOWN)
        {
            this->homeLocation[0] = downLocation[0];
            this->homeLocation[1] = downLocation[1];
            this->direction = DOWN;
        }
        else if (randomDirection == LEFT)
        {
            this->homeLocation[0] = leftLocation[0];
            this->homeLocation[1] = leftLocation[1];
            this->direction = LEFT;
        }
        else
        {
            this->homeLocation[0] = rightLocation[0];
            this->homeLocation[1] = rightLocation[1];
            this->direction = RIGHT;
        }
    }
}

void Creature::goHome(int mapWidth, int mapHeight)
{
    this->isGoingHome = true;
    this->setHomeLocation(mapWidth, mapHeight);
}

Food* Creature::isTouchingAnyFood(Food* foodsList, int listSize)
{
    for (int i=0; i<listSize; i++)
    {
        if (this->isTouching(foodsList[i]))
        {
            return &foodsList[i];
        }
    }
    return NULL;
}

int Creature::getNumberOfChilds()
{
    return this->childs.size();
}

void Creature::addChild(Creature *newChild)
{
    this->childs.push_back(newChild);
}

Creature* Creature::reproduce(int childID)
{
    int ID = childID;
    int new_x = this->x; int new_y = this->y; int new_Direction = this->direction;
    float new_speed; float new_secondUnitProb; float new_energy;
    float new_chngCoef = this->changeDirectionCoef; Creature* isCreatedBy = this;

    std::string Speed_Mu = std::to_string(this->speed);
    std::string Prob_Mu = std::to_string(this->secondUnitProb);
    std::string Energy_Mu = std::to_string(this->energy);
    std::string Speed_Sigma = "0.5";
    std::string Prob_Sigma = "0.25";
    std::string Energy_Sigma = "0.5";
    std::string Size = "1"; // Number of random data points from the 1D Gaussian.

    Py_Initialize();
    FILE* pythonScript; std::string line; std::fstream outputData1, outputData2, outputData3;

    // Calculating Speed
    remove(INPUT_NORMAL); std::ofstream inputData1(INPUT_NORMAL, std::ofstream::out); // This file is going to contain Mu, Sigma and Size.
    inputData1 << Speed_Mu << "\n"; inputData1 << Speed_Sigma << "\n"; inputData1 << Size;
    inputData1.close();
    pythonScript = fopen(RANDOM_NORMAL_SCRIPT, "r"); // Opening Python script.
    PyRun_AnyFile(pythonScript, RANDOM_NORMAL_SCRIPT); // Running it with Python3.
    fclose(pythonScript);
    outputData1.open(OUTPUT_NORMAL, std::ios::in); // File containing data generated by Python.
    getline(outputData1, line); // Reading each file line.
    new_speed = std::stof(line);

    // Calculating Probability
    remove(INPUT_NORMAL); std::ofstream inputData2(INPUT_NORMAL, std::ofstream::out); // This file is going to contain Mu, Sigma and Size.
    inputData2 << Prob_Mu << "\n"; inputData2 << Prob_Sigma << "\n"; inputData2 << Size;
    inputData2.close();
    pythonScript = fopen(RANDOM_NORMAL_SCRIPT, "r"); // Opening Python script.
    PyRun_AnyFile(pythonScript, RANDOM_NORMAL_SCRIPT); // Running it with Python3.
    fclose(pythonScript);
    outputData2.open(OUTPUT_NORMAL, std::ios::in); // File containing data generated by Python.
    getline(outputData2, line); // Reading each file line.
    new_secondUnitProb = std::stof(line);

    // Calculating Energy
    remove(INPUT_NORMAL); std::ofstream inputData3(INPUT_NORMAL, std::ofstream::out); // This file is going to contain Mu, Sigma and Size.
    inputData3 << Energy_Mu << "\n"; inputData3 << Energy_Sigma << "\n"; inputData3 << Size;
    inputData3.close();
    pythonScript = fopen(RANDOM_NORMAL_SCRIPT, "r"); // Opening Python script.
    PyRun_AnyFile(pythonScript, RANDOM_NORMAL_SCRIPT); // Running it with Python3.
    fclose(pythonScript);
    outputData3.open(OUTPUT_NORMAL, std::ios::in); // File containing data generated by Python.
    getline(outputData3, line); // Reading each file line.
    new_energy = std::stof(line);

    Creature* child = new Creature(ID, new_x, new_y, new_speed, new_secondUnitProb, new_energy, new_Direction, new_chngCoef, isCreatedBy);
    this->addChild(child);
    return child;
}
