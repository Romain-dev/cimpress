#include "cell.h"

Cell::Cell()
{
}

Cell::Cell(bool isAvailable, int value)
{
    this->isAvailable = isAvailable;
    this->value = value;
}
bool Cell::getIsAvailable()
{
    return this->isAvailable;
}

void Cell::setIsAvailable(bool isAvailable)
{
    this->isAvailable = isAvailable;
}
int Cell::getValue()
{
    return this->value;
}
void Cell::setValue(int value)
{
    this->value = value;
}
