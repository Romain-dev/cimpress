//GALLI Romain
//GHARBI Eddy
//INFRES-6

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
QTime timer;
void remplirGrilleAvecNouveauCarre(Square *carre, Cell** grid);
void afficherGrilleIsAvailable(Cell **grille);
void afficherGrilleValue(Cell **grille);
void lireFichier(char* path);
void explorerCelluleSuivante(int positionI, int positionJ);
void rechercherSolutionOptimale(int i, int j, Cell** currentGrid, int nbSquares);
void getPositionOfNextCase(Cell** grid, int result[2]);
void cloneGrid(Cell** gridOriginal, Cell** gridToClone);
void getPlusGrandCarre(int i, int j, Cell** currentGrille, Square* square);
void analysePossibilite();
int nbSquareProvisoire = 0;
int nbRecursions= 0;
int nbTrous = 0;
int nbVariables = 0;
bool flagStopTimer = false;

int main(int argc, char *argv[])
{

    lireFichier("../build/sample/s13.txt");
    analysePossibilite();

    cloneGrid(grille,grilleCloned);
    //afficherGrilleValue(grille);
    int position[2] = {-1,-1};
    getPositionOfNextCase(grille,position);
    explorerCelluleSuivante(position[0],position[1]);

    cout << "Solution de base: " << endl;
    afficherGrilleValue(grille);
    cout << "Nombre de carrées: " << nbSquareProvisoire << endl;


    timer.start();
    getPositionOfNextCase(grilleCloned,position);
    rechercherSolutionOptimale(position[0],position[1],grilleCloned,0);
    float difference = timer.elapsed();

    cout << endl;
    cout << "Solution Optimale: " << endl;
    afficherGrilleValue(grille);
    cout << "Hauteur : " << ligne << endl;
    cout << "Largeur : " << colonne << endl;
    cout << "Nombre trous : " << nbTrous << endl;
    cout << "Nombre variables : " << nbVariables << endl;
    cout << "Nombre de carrées: " << nbSquareProvisoire << endl;
    cout << "Nombre de récursions: " << nbRecursions << endl;
    if(flagStopTimer)
    {
        cout << "Temps de traitement superieur a 2 minutes, arret du programme" << endl;
    }
    cout << "Temps CPU : " << difference/1000 << " secondes" << endl;
    cout << endl;
    return 1;
}

//Permet de charger la contenu de fichier dans la matrice
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
            if(text.at(index).digitValue() == 1)
            {
                nbTrous++;
            }
            index ++;
        }

    }
}
//Affiche dans la console la grille avec les espaces disponibles
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
//Permet d'afficher le contenu de la grille dans la matrice
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
//Fonction qui sert à trouver une premiere solution, avant d'explorer l'arbre
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
//insertion du carre dans grid
void remplirGrilleAvecNouveauCarre(Square *carre, Cell** grid)
{
    //L'id du carre
    squareId ++;
    if(squareId >= 100) {
        squareId = 10;
    }

    //On insert le carre dans la grille
    for(int i=carre->getPositionI();i<carre->getLargeur() + carre->getPositionI();i++)
    {
        for(int j=carre->getPositionJ();j<carre->getLargeur() +carre->getPositionJ() ;j++)
        {
            grid[i][j].setValue(squareId);
        }
    }
}
//suppression du carre dans grid
void supprimerGrilleAvecCarre(Square *carre, Cell** grid)
{
    //Suppression du carre pour rendre la grille propre
    for(int i=carre->getPositionI();i<carre->getLargeur() + carre->getPositionI();i++)
    {
        for(int j=carre->getPositionJ();j<carre->getLargeur() +carre->getPositionJ() ;j++)
        {
            grid[i][j].setValue(0);
        }
    }
}
//Permet de trouver le plus grand carre possible a une position i,j dans la grille.
void getPlusGrandCarre(int i, int j, Cell** currentGrille, Square* square)
{
    int size = 1;
    bool canContinu = true;

    //C'est une case vide
    if(!currentGrille[i][j].getIsAvailable()) {
        return;
    }

    //On cherche le plus grand carre possible (on essai avec 1, puis 2, puis 3, ...)
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
//Fonction principale.
//C'est la fonction recursive qui permet de trouver la solution optimale.
void rechercherSolutionOptimale(int i, int j, Cell** currentGrid, int nbSquares)
{
    //On check l'horloge toutes les 5 secondes
    if(nbRecursions % 10000000 == 0 && timer.elapsed() > 120000)
    {
        flagStopTimer = true;
        return;
    }
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
    //Pour chaque carre de taille 1 a la taille max du carre possible a cette position
    for(int i = square->getLargeur(); i >= 1; i--)
    {
       Square *squareScoped = new Square(i,square->getPositionI(),square->getPositionJ());
       remplirGrilleAvecNouveauCarre(squareScoped,currentGrid);
       int coord[2] = {-1,-1};
       getPositionOfNextCase(currentGrid,coord);

       //C'est un noeud
       if(coord[0] != -1 && coord[1] != -1)
       {
           rechercherSolutionOptimale(coord[0], coord[1], currentGrid, nbSquares);
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
       //On nettoient la matrice pour depiler
       supprimerGrilleAvecCarre(square,currentGrid);
       delete(squareScoped);
    }
    delete(square);
}
//Donne la prochaine position i,j ou l'ont peut positionner un carre dans la grille
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
//Clone arg1 dans arg2
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
//Calcul nombre de variables binaires
void analysePossibilite()
{
    Square* square = new Square();
    for(int i=0;i<ligne;i++)
    {
        for(int j=0;j<colonne;j++)
        {
            //Uniquement si la case est disponible
            if(grille[i][j].getIsAvailable())
            {
                //Calcul a largeur du plus grand carré possible
                getPlusGrandCarre(i,j,grille,square);
                //Ajoute la largeur au nombre de variables binaires
                nbVariables += square->getLargeur();
            }
        }
    }
}
