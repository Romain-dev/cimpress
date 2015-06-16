#include "iostream"
#include <stdio.h>
#include "cell.h"
#include <QFile>
#include <QTextStream>

using namespace std;
int ligne, colonne;

Cell** grille;
void afficherGrilleIsAvailable(Cell **grille);
void afficherGrilleValue(Cell **grille);
void lireFichier(char* path);
void exploerCelluleSuivante(int positionI, int positionJ);
int main(int argc, char *argv[])
{
    lireFichier("../build/sample/s9.txt");
    afficherGrilleIsAvailable(grille);
    afficherGrilleValue(grille);
    exploerCelluleSuivante(0,0);
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

