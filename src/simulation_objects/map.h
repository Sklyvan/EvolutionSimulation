#ifndef SIMULATIONWORLD_MAP_H
#define SIMULATIONWORLD_MAP_H

#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "cell_information.h"

class Map: public ScreenObject
{
public:
    CellInformation* informationMatrix;
    size_t sizeBytes;

    Map() : ScreenObject()
    {
        this->informationMatrix = NULL;
        this->sizeBytes = 0;
    }
    Map(int width, int height) : ScreenObject(0, 0, 0, width, height)
    {
        this->informationMatrix = (CellInformation*) malloc(width * height * sizeof(CellInformation));
        this->sizeBytes = width * height * sizeof(CellInformation);
    }

    CellInformation* getCell(int i, int j);
    void initialize();
    void setCell(int i, int j, CellInformation setTo);
    void setFoodTo(int i, int j, Food* toSet);
    void setCreatureTo(int i, int j, Creature* toSet);
    void removeCreature(int i, int j);
    void removeFood(int i, int j);
    void clearCell(int i, int j);
    void clear();
    void clearCreatureFromCells();
    void clearFoodFromCells();

    bool saveMap(const char *filePath);
    bool loadMap(const char* filePath);
};

#endif //SIMULATIONWORLD_MAP_H
