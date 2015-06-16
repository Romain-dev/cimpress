#include "iostream"
#include <stdio.h>
#include "cell.h"
#include <QFile>
#include <QTextStream>
#include <square.h>

using namespace std;
int ligne, colonne;

Cell** grille;
void afficherGrilleIsAvailable(Cell **grille);
void afficherGrilleValue(Cell **grille);
void lireFichier(char* path);
void exploerCelluleSuivante(int positionI, int positionJ);
Square* getPlusGrandCarre(int i, int j, Cell** currentGrille);


int main(int argc, char *argv[])
{
    lireFichier("../build/sample/s9.txt");
    afficherGrilleIsAvailable(grille);
    afficherGrilleValue(grille);
    exploerCelluleSuivante(0,0);
    return 1;
}

Square* getPlusGrandCarre(int i, int j, Cell** currentGrille)
{
    int size = 1;
    bool canContinu = true;

    if(!currentGrille[i][j].getIsAvailable()) {
        return NULL;
    }

    while(canContinu)
    {

    }

    Square *square = new Square(size, i, j);
    return square;
}

void lireFichier(char* path)
{
    QFile inputSudoku(path);
    if(!inputSudoku.open(QIODevice::ReadOnly))
    {
            cout << "Error while openning input file" << endl;
            return;
    }

    QTextStream stream(&inputSudoku);
    QString text = stream.readLine();
    colonne = text.at(0).digitValue();
    text = stream.readLine();
    ligne = text.at(0).digitValue();
    text = stream.readLine(ligne*colonne*8);
    grille = new Cell*[ligne];
    for(int i=0;i<ligne;i++)
    {
        grille[i] = new Cell[colonne];
    }
    int index = 0;
    for(int i=0;i<ligne;i++)
    {
        for(int j=0;j<colonne;j++)
        {
            Cell cellule(!text.at(index).digitValue(), 0);
            grille[i][j] = cellule;
            index ++;
        }

    }
}
void afficherGrilleIsAvailable(Cell **grille)
{
    cout << "Affichage matrice " << endl;
    for(int i=0;i<ligne;i++)
    {
        for(int y=0;y<colonne;y++)
        {
            cout << grille[i][y].getIsAvailable() <<  " ";
        }
        cout << endl;
    }
}
void afficherGrilleValue(Cell **grille)
{
    cout << "Affichage matrice " << endl;
    for(int i=0;i<ligne;i++)
    {
        for(int y=0;y<colonne;y++)
        {
            if(grille[i][y].getIsAvailable())
            {
                cout << grille[i][y].getValue() <<  " ";
            }
            else{
                cout << "  ";
            }
        }
        cout << endl;
    }
}
void exploerCelluleSuivante(int positionI, int positionJ)
{
    if(positionI < ligne)
    {
        if(positionJ < colonne)
        {
            if(grille[positionI][positionJ].getIsAvailable())
            {
                cout << "Explore" << endl;
                exploerCelluleSuivante(positionI,positionJ);
            }
        }
        else{
            positionI++;
        }
    }
}
void remplirGrilleAvecNouveauCarre(Square *carre)
{
    for(int i=carre->getPositionI();i<carre->getLargeur();i++)
    {
        for(int j=carre->getPositionJ();j<carre->getLargeur();j++)
        {
            grille[i][j].setValue(1);
        }
    }
    afficherGrilleValue(grille);
}
