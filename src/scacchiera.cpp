/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */ 

#include "include/board.h"
#include "include/match/player.hpp"
#include "include/match/match.hpp"
#include "include/log/logger.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std; 
string const console = "console";

void playerGame() 
{
    board b();
    logger logger;
    logger.log(console, "Welcome");
    logger.log(console, "Starting new log session");
    //continuo del log

    string username;
    string computer{"computer"};
    cout << "Enter a username : ";
    cin >> username;

    player p1(username, false);
    player pc(computer, true);
    match sus(p1, computer, b); 
    logger.log(console, "Initializing player 1 " + username);
    logger.log(console, "Initializing player 2 " + computer);

    int starter = rand() % 1;
    cout << starter;
    p1 p2 int b
    //problems with board.h


}
    
    
    
}

void computersGame() {}

int main() 
{
    srand(time(NULL));
    bool start = false;
    char enter;
    cout << "  ______  __    __   _______      _______.     _______. \n" << 
            " /      ||  |  |  | |   ____|    /       |    /       | \n" << 
            "|  ,----'|  |__|  | |  |__      |   (----`   |   (----` \n" << 
            "|  |     |   __   | |   __|      \\   \\        \\   \\ \n" << 
            "|  `----.|  |  |  | |  |____ .----)   |   .----)   |    \n" << 
            " \\______||__|  |__| |_______||_______/    |_______/    \n" <<
            "\n   WELCOME! PRESS X TO START THE GAME, 1 TO EXIT      \n";
    while(!start) 
    {
        cin >> enter;
        if( enter == 'X' || enter == 'x')
        {
            system("cls");
            start = true;
        }    
        else if(enter == '1')
            exit(1);
        else
            cout << "Invalid command! PRESS ENTER TO PLAY, 1 TO EXIT" << endl;
    }
    start = false;

    cout << "1: Simulate a match (PC vs PC) \n" <<
            "2: Play a real time match (You vs PC) \n";
    while(!start) 
    {
        cin >> enter;
        if(enter == '1')
        {
            system("cls");
            computersGame();
            start = true;
        }    
        else if(enter == '2')
            playerGame();
        else
        {
            cout << "You can do it! 1 or 2 : ";
        }
    }
    return 0;
}


