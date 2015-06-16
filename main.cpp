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
int main(int argc, char *argv[])
{ 
    QFile inputSudoku("../build/sample/s9.txt");
    if(!inputSudoku.open(QIODevice::ReadOnly))
    {
            cout << "Error while openning input file" << endl;
            return -1;
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
    afficherGrilleIsAvailable(grille);
    afficherGrilleValue(grille);
    return 1;
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
