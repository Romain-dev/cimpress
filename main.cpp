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
void afficherGrilleIsAvailable(Cell **grille);
void afficherGrilleValue(Cell **grille);
void lireFichier(char* path);
void explorerCelluleSuivante(int positionI, int positionJ);
void rechercherSolutionOptimale(int positionI, int positionJ);
Square* getPlusGrandCarre(int i, int j, Cell** currentGrille);
int currentSolutionNbSquare = 0;
int bestSolutionNbSquare = 0;
Cell** grilleCurrentSolution;
void viderGrille(Cell** grille);
int main(int argc, char *argv[])
{
    lireFichier("../build/sample/s2.txt");
    afficherGrilleIsAvailable(grille);
    afficherGrilleValue(grille);
    //explorerCelluleSuivante(0,0);
    rechercherSolutionOptimale(0,0);
    afficherGrilleValue(grille);
    cout << "Best Solution Nb Square : " << bestSolutionNbSquare << endl;
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
    ligne = 12;
    colonne = 10;
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
    grilleCurrentSolution = new Cell*[ligne];
    for(int i=0;i<ligne;i++)
    {
        grilleCurrentSolution[i] = new Cell[colonne];
    }
    for(int i=0;i<ligne;i++)
    {
        for(int j=0;j<colonne;j++)
        {
            grilleCurrentSolution[i][j] = grille[i][j];
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
                getPlusGrandCarre(positionI, positionJ, grilleCurrentSolution);
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
void remplirGrilleAvecNouveauCarre(Square *carre)
{
    squareId ++;
    if(squareId == 10) {
        squareId = 1;
    }

    for(int i=carre->getPositionI();i<carre->getLargeur() + carre->getPositionI();i++)
    {
        for(int j=carre->getPositionJ();j<carre->getLargeur() +carre->getPositionJ() ;j++)
        {
            grille[i][j].setValue(squareId);
        }
    }
    //afficherGrilleValue(grille);
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
    remplirGrilleAvecNouveauCarre(square);
    rechercherSolutionOptimale(i, j);
    currentSolutionNbSquare++;
    return square;
}
void rechercherSolutionOptimale(int positionI, int positionJ)
{
    if(positionI < ligne)
    {
       if(positionJ < colonne)
        {
             if(grille[positionI][positionJ].getIsAvailable() && grille[positionI][positionJ].getValue() == 0)
            {
                viderGrille(grille);
                getPlusGrandCarre(positionI, positionJ, grille);
                explorerCelluleSuivante(positionI,++positionJ);
                cout << "Current Solution Nb Square : " << currentSolutionNbSquare << endl;
                if(currentSolutionNbSquare < bestSolutionNbSquare || currentSolutionNbSquare == 0 || bestSolutionNbSquare == 0)
                {
                    bestSolutionNbSquare = currentSolutionNbSquare;
                    cout << "Current Solution Nb Square : " << bestSolutionNbSquare << endl;
                }
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
void viderGrille(Cell** grille)
{
    for(int i=0;i<ligne;i++)
    {
        for(int j=0;j<colonne;j++)
        {
            grille[i][j].setValue(0);
        }
    }
}
