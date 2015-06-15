#include "iostream"
using namespace std;

int grille[9][9];
int main(int argc, char *argv[])
{
    int i,j;

    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            cin >> grille[i][j];
        }
    }
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<9;j++)
        {
            cout << grille[i][j] << ' ';
        }
        cout << endl;
    }
    return 1;
}
