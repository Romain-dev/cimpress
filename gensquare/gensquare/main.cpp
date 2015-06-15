#include "iostream"
#include "stdlib.h"
#include "time.h"

using namespace std;
#define DIMMAX 20

char puzzle[DIMMAX][DIMMAX];
int largeur,hauteur,nbtrous;

int main(int argc, char *argv[])
{
    if(argc != 4) {
        cout << "1) largeur\n2) hauteur\n3) nb max de trous" << endl;
        return 1;
    }

    largeur = atoi(argv[1]);
    hauteur = atoi(argv[2]);
    nbtrous = atoi(argv[3]);

    for (int i = 0; i < hauteur; i++)
    {
        for (int j = 0; j < largeur; j++)
        {
            puzzle[i][j] = 0;
        }
    }

    srand((int)time(0));

    return 1;
}
