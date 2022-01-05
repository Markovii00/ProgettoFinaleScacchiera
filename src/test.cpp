#include "board.cpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std; 
string const console = "console";

void playerGame() 
{
    board b;
    bool moved = b.move(0,1,2,2);
    cout << "porcodio";
    if(moved){
        b.printBoard();
    }
    else 
        cout << "suca";

    cout << "porcodio";
    cout << "porcodio";

    
}
    


int main() 
{
    playerGame();
    return 0;
}
