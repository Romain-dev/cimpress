#ifndef CELL_H
#define CELL_H

class Cell
{
public:
    Cell(bool isAvailable, int value);
    Cell();
    bool isAvailable = true;
    int value = 0;
};

#endif // CELL_H
