#include "cell.h"

Cell::Cell()
{
}

Cell::Cell(bool isAvailable, int value)
{
    this->isAvailable = isAvailable;
    this->value = value;
}

