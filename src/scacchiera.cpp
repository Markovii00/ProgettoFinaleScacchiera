/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */ 

#include "board.cpp"
#include "include/match/player.h"
#include "include/match/match.hpp"
#include "include/match/user.hpp"
#include "include/match/bot.cpp"
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
    
    string drawCondition = "FF";
    string to_log;
    string writeTo = " to ";
    short setSite;
    
    bool turn;
    bool userRequestedDraw = false;
    bool botAcceptedDraw = false;
    bool userAccpetedDraw = false;
    bool botRquestedDraw = false;

    pair <bool, bool> moveOutput;
    pair <bool,bool> ended = make_pair(false,false);
    pair <bool, bool> invalidMove = make_pair(false, true);
    pair <bool, bool> draw = make_pair(true, false);
    logger logger;
    regex input_filter("^([a-hA-H]){1}([1-8]){1} ([a-hA-H]){1}([1-8]){1}$");
    smatch coordinates;
    vector<string> botNames = {"Zincalex", "Colla", "Markovii", "Yasuo", "Yone", "MapBot"};
    coords start;
    coords end;


    logger.log(console, "Welcome");
    logger.log(console, "Starting new log session");
    //continuo del log

    string username;
    string computer{"computer"};
    cout << "Enter a username : ";
    cin >> username;
    cin.ignore();

    user p1(username, false);
    bot bot1(computer, true, b);
    bot1.set_name(botNames);
    
    logger.log(console, "Initializing player 1 " + p1.get_name());
    logger.log(console, "Initializing player 2 " + bot1.get_name());

    int starter = rand() % 2;
    
    match game(p1, bot1, b, starter); 
    logger.log(console, "Creating match");
    logger.log(console, "Starting match");

    //game start
    while(moveOutput != ended) {
        turn = game.whose_turn();   
        if(game.whose_turn()) { //player turn 
            system("cls");
            b.printBoard();
            cout << '\n' << "Last move: " << to_log;
            while(moveOutput == invalidMove){
                do {
                    if(botRquestedDraw == true){
                        cout << bot1.get_name() << " ha richiesto la patta, vuoi accettarla? Y/N";
                        getline(cin, input);
                        if(input == "Y" || input == "y"){
                            moveOutput = draw;
                            logger.log(game.get_player_turn().get_name(), "Accepted the draw");
                        }
                    }
                    if(moveOutput == draw){
                        cout << "The game ended with a draw!";
                        logger.log(console, "Draw, game is ended");
                        logger.log(console, "Ending log session");
                        return;
                    }
                    cout << "\n\nINSERT MOVE : ";
                    getline(cin, input);
                    if(input == drawCondition){
                        userRequestedDraw = true;
                        break;
                    }
                    regex_search(input, coordinates, input_filter);
                }while(!regex_match(input, input_filter));
                if(userRequestedDraw == false) {

                    short *mosse = conversion(coordinates);
                    start.first = *(mosse + 0);
                    start.second = *(mosse + 1);
                    end.first = *(mosse + 2);
                    end.second = *(mosse + 3);

                    b.move(start, end, turn);
                }
            }
            if(userRequestedDraw == false) {
                to_log = "Moving " + (char) start.first + (char) start.second + writeTo + (char) end.first +
                         (char) end.second;
                logger.log(game.get_player_turn().get_name(), to_log);
            } else
                logger.log(game.get_player_turn().get_name(), "requested draw");
        }
        else {//pc turn
             cout << '\n' << "Last move: " << to_log;
             if(userRequestedDraw == false){
                while(moveOutput == invalidMove){

                     botRquestedDraw = bot1.requestDraw();
                    if(botRquestedDraw == false){
                        start = bot1.generateFromCoords();
                        end = bot1.generateEndCoords(start);
                        b.move(start, end, turn);
                    }else{
                        logger.log(game.get_player_turn().get_name(), "requested draw");
                        break;
                    }
                }
             }else botAcceptedDraw=bot1.handledraw();

             if(botAcceptedDraw){
                 logger.log(game.get_player_turn().get_name(), "Accepted the draw");
                 moveOutput = draw;
             }
             to_log = "Moving " + (char)start.first + (char)start.second + writeTo + (char)end.first + (char)end.second;
             logger.log(game.get_player_turn().get_name(), to_log);
        }  
    }
    return;
    //METTERE UN IF PER VERIFICARE IL COLORE DELLE PEDINE CHE CHIAMANO IL MOVE PER NON MUOVERE PEZZI AVVERSAR//proponga
}
    
void computersGame() {
    board b;
    string to_log;
    string writeTo = " to ";
    bool turn;
    pair <bool, bool> moveOutput;
    pair <bool,bool>ended = make_pair(false,false);
    pair <bool, bool>invalidMove = make_pair(false, true);
    logger logger;
    coords start;
    coords end;

    vector<string> botNames = {"Zincalex", "Colla", "Markoovii", "Yasuo", "Yone", "MapBot", ""};

    bot bot1(writeTo , false, b);
    bot bot2(writeTo , false, b);

    bot1.set_name(botNames);
    bot2.set_name(botNames);

    logger.log(console, "Initializing player 1 " + bot1.get_name());
    logger.log(console, "Initialising player 2 " + bot2.get_name());

    int starter = rand() % 2;

    match game(bot1, bot2, b, starter);

    logger.log(console, "Creating match");
    logger.log(console, "Starting match");

    while(moveOutput!= ended){
        turn = game.whose_turn();
        if(turn){
            system("cls");
            b.printBoard();

            while (moveOutput == invalidMove){
            start = bot1.generateFromCoords();
            end = bot1.generateEndCoords(start);
            
            b.move(start, end, turn);
            }

            to_log = "Moving " + (char)start.first + (char)start.second + writeTo + (char)end.first + (char)end.second;
            logger.log(game.get_player_turn().get_name(), to_log);
            
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        else{
            system("cls");
            b.printBoard();
            while (moveOutput == invalidMove)
            {
                
                start = bot2.generateFromCoords();
                end = bot2.generateEndCoords(start);
            
                moveOutput = b.move(start, end, turn);
            }
            
            to_log = "Moving " + (char)start.first + (char)start.second + writeTo + (char)end.first + (char)end.second;
            logger.log(game.get_player_turn().get_name(), to_log);
            
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
    return;
}

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