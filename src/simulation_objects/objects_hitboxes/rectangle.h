#ifndef SIMULATIONWORLD_RECTANGLE_H
#define SIMULATIONWORLD_RECTANGLE_H


class Rectangle
{
public:
    int x, y;
    int width, height;

    Rectangle();
    Rectangle(int x, int y, int w, int h);

    bool isColliding(Rectangle toTest);
    bool isContaining(Rectangle toTest);
};


#endif //SIMULATIONWORLD_RECTANGLE_H
