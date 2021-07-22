#include "map.h"

void Map::initialize()
{
    this->clear();
}

CellInformation* Map::getCell(int i, int j)
{
    if (i < 0 || j < 0 || i >= this->w || j >= this->h)
    {
        write(2, "IndexError: Cell index out of range. \n", sizeof("IndexError: Cell index out of range. \n"));
        return NULL;
    }
    else return &this->informationMatrix[i*this->h + j];
}

void Map::setCell(int i, int j, CellInformation setTo)
{
    if (i < 0 || j < 0 || i >= this->w || j >= this->h)
    {
        write(2, "IndexError: Cell index out of range. \n", sizeof("IndexError: Cell index out of range. \n"));
    }
    else this->informationMatrix[i*this->h + j] = setTo;
}

void Map::setFoodTo(int i, int j, Food* toSet)
{
    this->getCell(i, j)->setFood(toSet);
}

void Map::setCreatureTo(int i, int j, Creature *toSet)
{
    this->getCell(i, j)->setCreature(toSet);
}

void Map::removeCreature(int i, int j)
{
    this->getCell(i, j)->removeCreature();
}

void Map::removeFood(int i, int j)
{
    this->getCell(i, j)->removeFood();
}

void Map::clearCell(int i, int j)
{
    this->getCell(i, j)->clear();
}

void Map::clear()
{
    for (int i=0; i<this->w; i++) for (int j=0; j<this->h; j++)
    {
        this->clearCell(i, j);
    }
}

void Map::clearCreatureFromCells()
{
    for (int i=0; i<this->w; i++) for (int j=0; j<this->h; j++)
    {
        this->getCell(i, j)->removeCreature();
    }
}
void Map::clearFoodFromCells()
{
    for (int i=0; i<this->w; i++) for (int j=0; j<this->h; j++)
    {
        this->getCell(i, j)->removeFood();
    }
}

bool Map::saveMap(const char *filePath)
{
    remove(filePath);
    int FD = open(filePath, O_CREAT | O_RDWR, S_IRWXU | S_IRWXO | S_IRWXG);
    if (FD != -1)
    {
        write(FD, this->informationMatrix, this->sizeBytes);
        close(FD);
        return true;
    }
    else
    {
        return false;
    }
}

bool Map::loadMap(const char *filePath)
{
    int FD = open(filePath, O_RDONLY);
    if (FD != -1)
    {
        read(FD, this->informationMatrix, this->sizeBytes);
        close(FD);
        return true;
    }
    else
    {
        return false;
    }
}