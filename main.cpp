#include "iostream"
#include <stdio.h>
#include "cell.h"
#include <QFile>
#include <QTextStream>
#include <square.h>

using namespace std;
int ligne, colonne;
int squareId = 0;
Cell** grille;
Cell** grilleCloned;
void afficherGrilleIsAvailable(Cell **grille);
void afficherGrilleValue(Cell **grille);
void lireFichier(char* path);
void explorerCelluleSuivante(int positionI, int positionJ);
void rechercherSolutionOptimale(int i, int j, int squareMaxSize, Cell** currentGrid, int nbSquares);
void getPositionOfNextCase(Cell** grid, int result[2]);
void cloneGrid(Cell** gridOriginal, Cell** gridToClone);
Square* getPlusGrandCarre(int i, int j, Cell** currentGrille);
int nbSquareProvisoire = 0;

int main(int argc, char *argv[])
{
    lireFichier("../build/sample/new/s1.txt");
    cloneGrid(grille,grilleCloned);
    afficherGrilleValue(grille);
    explorerCelluleSuivante(0,0);
    afficherGrilleValue(grille);

    rechercherSolutionOptimale(0,0,20,grilleCloned,0);
    return 1;
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

    grilleCloned = new Cell*[ligne];
    for(int i=0;i<ligne;i++)
    {
        grilleCloned[i] = new Cell[colonne];
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
void explorerCelluleSuivante(int positionI, int positionJ)
{
    if(positionI < ligne)
    {
        if(positionJ < colonne)
        {
            if(grille[positionI][positionJ].getIsAvailable() && grille[positionI][positionJ].getValue() == 0)
            {
                getPlusGrandCarre(positionI, positionJ, grille);
                nbSquareProvisoire++;
                explorerCelluleSuivante(positionI,++positionJ);

            }
            else{
                positionJ++;
                explorerCelluleSuivante(positionI,positionJ);
            }
        }
        else{
            positionI++;
            positionJ = 0;
            explorerCelluleSuivante(positionI,positionJ);
        }
    }
}
void remplirGrilleAvecNouveauCarre(Square *carre, Cell** grid)
{
    squareId ++;
    if(squareId == 10) {
        squareId = 1;
    }

    for(int i=carre->getPositionI();i<carre->getLargeur() + carre->getPositionI();i++)
    {
        for(int j=carre->getPositionJ();j<carre->getLargeur() +carre->getPositionJ() ;j++)
        {
            grid[i][j].setValue(squareId);
        }
    }
}
void supprimerGrilleAvecCarre(Square *carre, Cell** grid)
{
    squareId --;
    if(squareId == 1) {
        squareId = 9;
    }

    for(int i=carre->getPositionI();i<carre->getLargeur() + carre->getPositionI();i++)
    {
        for(int j=carre->getPositionJ();j<carre->getLargeur() +carre->getPositionJ() ;j++)
        {
            grid[i][j].setValue(0);
        }
    }
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
        if (i+size < ligne && j+size < colonne)
        {
            bool isOk = true;
            for(int ii = i; ii<= i+size; ii++)
            {
                for(int jj = j; jj<= j+size; jj++)
                {
                    if(!currentGrille[ii][jj].getIsAvailable() || currentGrille[ii][jj].getValue()!=0)
                    {
                        isOk = false;
                        break;
                    }
                }
            }

            if(isOk)
            {
                size++;
            }
            else{
                canContinu = false;
            }
        }
        else
        {
            canContinu = false;
        }
    }

    Square *square = new Square(size, i, j);
    remplirGrilleAvecNouveauCarre(square,currentGrille);
    return square;
}
void rechercherSolutionOptimale(int i, int j, int squareMaxSize, Cell** currentGrid, int nbSquares)
{
    //Couper la branche
    if(nbSquares >= nbSquareProvisoire)
    {
        return;
    }

    Square *square = getPlusGrandCarre(i,j,currentGrid);
    afficherGrilleValue(currentGrid);
    int coord[2] = {-1,-1};
    getPositionOfNextCase(currentGrid,coord);

    if(coord[0] != -1 && coord[1] != -1)
    {
        rechercherSolutionOptimale(coord[0], coord[1], squareMaxSize, currentGrid, ++nbSquares);
    }

    supprimerGrilleAvecCarre(square,currentGrid);
    afficherGrilleValue(currentGrid);
}
void getPositionOfNextCase(Cell** grid, int result[2])
{
    for(int i=0;i<ligne;i++)
    {
        for(int y=0;y<colonne;y++)
        {
            if(grid[i][y].getValue() == 0 && grid[i][y].getIsAvailable())
            {
                result[0] = i;
                result[1] = y;
                return;
            }
        }
    }

    return;
}
void cloneGrid(Cell** gridOriginal, Cell** gridToClone)
{
    for(int i=0;i<ligne;i++)
    {
        for(int y=0;y<colonne;y++)
        {
            Cell *cellule = new Cell();
            cellule->setIsAvailable(gridOriginal[i][y].getIsAvailable());
            cellule->setValue(gridOriginal[i][y].getValue());

            gridToClone[i][y] = *cellule;
        }
    }
}
