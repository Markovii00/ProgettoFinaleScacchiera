#include "board.cpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std; 
string const console = "console";

short* conversion(string _m) 
{
    if()
    short fromCol = ((int)_m[0]-65);
    short fromRow = ((int)_m[1]-8);
    short toCol = ((int)_m[3]-65);
    short toRow = ((int)_m[4]-8);
    
    short mosse[4] = {fromRow, fromCol, toRow, toCol};
    short* p = mosse;
    return p;
}

void playerGame() 
{
    board b;
    string _coordinates;
    b.printBoard();
    cout << endl;
    cout << "Inserisci mossa : ";
    cin >> _coordinates;
    short *mosse = conversion(_coordinates);
   
    bool moved = b.move(mosse[0], mosse[1], mosse[2], mosse[3]);
   

    b.printBoard(); 
}

int main() 
{
    playerGame();
    return 0;
}
