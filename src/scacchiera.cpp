/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */ 

#include "board.cpp"
#include "include/match/player.hpp"
#include "include/match/match.hpp"
#include "include/log/logger.cpp"
#include <iostream>
#include <cctype>
#include <chrono>
#include <thread>
#include <string>
#include <algorithm>
#include <regex>

using namespace std; 
string const console = "console";

//Convert input regex in matrix coordinates
short* conversion(smatch& m) 
{
    short fromCol = toupper(m.str(1)[0]) - 65;
    short fromRow = 8 - stoi(m.str(2));
    short toCol = toupper(m.str(3)[0]) - 65;
    short toRow = 8 - stoi(m.str(4));

    short ret[4] = {fromRow, fromCol, toRow, toCol};
    short *array = &ret[0];
    return array; 
}

void playerGame() 
{
    board b;
    string input;
    logger logger;
    regex input_filter("^([a-hA-H]){1}([1-8]){1} ([a-hA-H]){1}([1-8]){1}$");
    smatch coordinates;

    logger.log(console, "Welcome");
    logger.log(console, "Starting new log session");
    //continuo del log

    string username;
    string computer{"computer"};
    cout << "Enter a username : ";
    cin >> username;
    cin.ignore();

    player p1(username, false);
    player pc(computer, true);
    
    logger.log(console, "Initializing player 1 " + p1.get_name());
    logger.log(console, "Initializing player 2 " + pc.get_name());

    int starter = rand() % 2;
    //PER MARKOVI SE STARTER = 0, DEVI SETTARE LA TUA VARIBILE ROUND = STARTER
    //0 INIZIA IL PLAYER, 1 STARTA IL PC
    match game(p1, pc, b, starter); 
    logger.log(console, "Creating match");
    logger.log(console, "Starting match");

    //game start
    while(!b.endGame()) {   
        if(game.whose_turn()) { //player turn 
            b.printBoard();
            do {
                cout << "\n\nINSERT MOVE : ";
                getline(cin, input);
                regex_search(input, coordinates, input_filter);
            } while(!regex_match(input, input_filter));
            
            short *mosse = conversion(coordinates);
            short fRow = *(mosse + 0);
            short fCol = *(mosse + 1);
            short toRow = *(mosse + 2);
            short toCol = *(mosse + 3);
            

        }
        else { //pc turn

        }
        
    }
   
   

 
    //METTERE UN IF PER VERIFICARE IL COLORE DELLE PEDINE CHE CHIAMANO IL MOVE PER NON MUOVERE PEZZI AVVERSARI 


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
    do 
    {
        cin >> enter;
        if(enter == 'X' || enter == 'x')
            system("cls"); 
        else if(enter == '1')
            exit(1);
        else cout << "Invalid command! PRESS X TO PLAY, 1 TO EXIT" << endl;
    } while(((enter != 'X') && !(enter != 'X')) || (!(enter != 'X') && (enter != 'X')));
    
    cout << "1: Simulate a match (PC vs PC) \n" <<
            "2: Play a real time match (You vs PC) \n";
    do 
    {
        cin >> enter;
        if(enter == '1')
        {
            system("cls");
            computersGame();
            start = true;
        }    
        else if(enter == '2')
        {
            system("cls");
            playerGame();
            start = true;
        }
        else cout << "You can do it! 1 or 2 : ";   
    } while(!start);

    return 0;
}





