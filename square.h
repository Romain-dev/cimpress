#ifndef SQUARE_H
#define SQUARE_H

class Square
{
public:
    Square();
private:
    int largeur;
    int positionI;
    int positionJ;
public:
    int getLargeur();
    void setLargeur(int largeur);
    int getPositionI();
    void setPositionI(int positionI);
    int getPositionJ();
    void setPositionJ(int positionJ);
};

#endif // SQUARE_H
