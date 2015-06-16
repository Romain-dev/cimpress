#ifndef CELL_H
#define CELL_H

class Cell
{
public:
    Cell(bool isAvailable, int value);
    Cell();
private:
    bool isAvailable = true;
    int value = 0;
public:
    bool getIsAvailable();
    void setIsAvailable(bool isAvailable);
    int getValue();
    void setValue(int value);
};

#endif // CELL_H
