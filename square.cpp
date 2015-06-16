#include "square.h"

Square::Square()
{
}
Square::Square(int largeur, int positionI, int positionJ)
{
    this->largeur = largeur;
    this->positionI = positionI;
    this->positionJ = positionJ;
}

int Square::getLargeur()
{
    return this->largeur;
}
void Square::setLargeur(int largeur)
{
    this->largeur = largeur;
}
int Square::getPositionI()
{
    return this->positionI;
}
void Square::setPositionI(int positionI)
{
    this->positionI = positionI;
}
int Square::getPositionJ()
{
    return this->positionJ;
}
void Square::setPositionJ(int positionJ)
{
    this->positionJ = positionJ;
}
