#include "iostream"
#include <stdio.h>
#include "cell.h"
#include <QFile>
#include <QTextStream>
#include <square.h>
#include <QTime>

using namespace std;
int ligne, colonne;
int squareId = 10;
Cell** grille;
Cell** grilleCloned;
void remplirGrilleAvecNouveauCarre(Square *carre, Cell** grid);
void afficherGrilleIsAvailable(Cell **grille);
void afficherGrilleValue(Cell **grille);
void lireFichier(char* path);
void explorerCelluleSuivante(int positionI, int positionJ);
void rechercherSolutionOptimale(int i, int j, int squareMaxSize, Cell** currentGrid, int nbSquares);
void getPositionOfNextCase(Cell** grid, int result[2]);
void cloneGrid(Cell** gridOriginal, Cell** gridToClone);
void getPlusGrandCarre(int i, int j, Cell** currentGrille, Square* square);
int nbSquareProvisoire = 0;
int nbRecursions= 0;

int main(int argc, char *argv[])
{
    lireFichier("../build/sample/s4.txt");
    cloneGrid(grille,grilleCloned);
    //afficherGrilleValue(grille);
    int position[2] = {-1,-1};
    getPositionOfNextCase(grille,position);
    explorerCelluleSuivante(position[0],position[1]);

    cout << "Solution de base: " << endl;
    afficherGrilleValue(grille);
    cout << "Nombre de carrées: " << nbSquareProvisoire << endl;

    QTime time;
    time.start();
    getPositionOfNextCase(grilleCloned,position);
    rechercherSolutionOptimale(position[0],position[1],20,grilleCloned,0);
    float difference = time.elapsed();
    cout << endl;
    cout << "Solution Optimale: " << endl;
    afficherGrilleValue(grille);
    cout << "Nombre de carrées: " << nbSquareProvisoire << endl;
    cout << "Nombre de récursions: " << nbRecursions << endl;
    cout << "Temps CPU : " << difference/1000 << " secondes" << endl;
    cout << endl;
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
    QString text = stream.readLine(sizeof(int));
    colonne = text.toInt();
    text = stream.readLine(sizeof(int));
    ligne = text.toInt();
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
    for(int i=0;i<ligne;i++)
    {
        for(int y=0;y<colonne;y++)
        {
            if(grille[i][y].getIsAvailable())
            {
                cout << grille[i][y].getValue() <<  " ";
            }
            else{
                cout << "   ";
            }
        }
        cout << endl;
    }
}
void explorerCelluleSuivante(int positionI, int positionJ)
{
    Square *square = new Square();
    if(positionI < ligne)
    {
        if(positionJ < colonne)
        {
            if(grille[positionI][positionJ].getIsAvailable() && grille[positionI][positionJ].getValue() == 0)
            {
                getPlusGrandCarre(positionI, positionJ, grille, square);
                remplirGrilleAvecNouveauCarre(square,grille);
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
    if(squareId >= 100) {
        squareId = 10;
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

    for(int i=carre->getPositionI();i<carre->getLargeur() + carre->getPositionI();i++)
    {
        for(int j=carre->getPositionJ();j<carre->getLargeur() +carre->getPositionJ() ;j++)
        {
            grid[i][j].setValue(0);
        }
    }
}
void getPlusGrandCarre(int i, int j, Cell** currentGrille, Square* square)
{
    int size = 1;
    bool canContinu = true;

    if(!currentGrille[i][j].getIsAvailable()) {
        return;
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

    square->setLargeur(size);
    square->setPositionI(i);
    square->setPositionJ(j);
}
void rechercherSolutionOptimale(int i, int j, int squareMaxSize, Cell** currentGrid, int nbSquares)
{
    nbRecursions++;
    Square *square = new Square();
    //Couper la branche
    if(nbSquares >= nbSquareProvisoire)
    {
        delete(square);
        return;
    }

    getPlusGrandCarre(i,j,currentGrid, square);
    nbSquares++;
    for(int i = square->getLargeur(); i >= 1; i--)
    {
       Square *squareScoped = new Square(i,square->getPositionI(),square->getPositionJ());
       remplirGrilleAvecNouveauCarre(squareScoped,currentGrid);
       int coord[2] = {-1,-1};
       getPositionOfNextCase(currentGrid,coord);

       //C'est un noeud
       if(coord[0] != -1 && coord[1] != -1)
       {
           rechercherSolutionOptimale(coord[0], coord[1], squareMaxSize, currentGrid, nbSquares);
       }
       //C'est une feuille solution optimale
       else
       {
            cout << endl;
            cout<<"NouvelleSolutionOptimale"<<endl;
            nbSquareProvisoire = nbSquares;

            cloneGrid(currentGrid,grille);
            afficherGrilleValue(grilleCloned);
            cout << "Nombre de carrées: " << nbSquareProvisoire << endl;
            nbSquares--;

       }
       supprimerGrilleAvecCarre(square,currentGrid);
       delete(squareScoped);
    }
    delete(square);
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
