/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */ 

#include "board.cpp"
#include "include/log/logger.cpp"
#include "bot.cpp"
#include <iostream>
#include <cctype>
#include <chrono>
#include <thread>
#include <string>
#include <algorithm>
#include <regex>

using namespace std; 
string const console = "console";

string randomName(){
    vector<string> names = {"AMOGUS", "Baymax", "MarkoviiIsAFurry", "BakaBot", "Wall-E", "MapBot", "OwOBot", "SUSBot", "Bixby", "Cortana", "Alexa"};
    short namesLenght = names.size();
    short nameindex = rand() % namesLenght;
    std::string nameToSet = names.at(nameindex);

    return nameToSet;
}

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

void playerGame() {
    //INIZIALIZING VARIABLES
    board b;
    coords start;
    coords end;
    bool turn;
    pair <bool, int> moveOutput;
    pair<coords, coords> botMove;
    logger logger;
    string input;
    regex input_filter("^([a-hA-H]){1}([1-8]){1} ([a-hA-H]){1}([1-8]){1}$");
    smatch coordinates;
    char promotionChar;
    vector<char> promotionSet{'d', 'a', 't', 'c'};
    


    //All move possibilities, go look board.h file in order to see all return conditions
    //executed move
    pair <bool, int> succesfulMove = make_pair(true, 1);
    pair <bool, int> promotionCondition = make_pair(true, 2);
    pair <bool, int> checkMate = make_pair(true, 3);
    //move failed
    pair <bool, int> invalidMove = make_pair(false, 1);
    pair <bool, int> stillInCheck = make_pair(false, 2);
    pair <bool, int> threeFoldDrawCondition = make_pair(false, 3);
    pair <bool, int> drawGameOver = make_pair(false, 4);
    
    string drawCondition = "FF";
    string to_log = " ";
    string writeTo = " to ";

    bool userRequestedDraw = false;
    bool botAcceptedDraw = false;
    bool userAccepetedDraw = false;
    bool botRequestedDraw = false;
    
    //STARTING THE PROGRAM
    logger.log(console, "Welcome");
    logger.log(console, "Starting new log session");

    string p1;
    cout << "Enter a username : ";
    cin >> p1;
    cin.ignore();

    int starter = rand() % 2;
    if(starter = 1) turn = false; //bot will make first move
    else turn = true; //player will make first move
    
    string botname = randomName();

    bot bot(botname, true, b);
    
    logger.log(console, "Initializing player 1 \"" + p1 + "\"");
    logger.log(console, "Initializing player 2 \"" + bot.get_name() + "\"");
    
    logger.log(console, "Starting match");

    //GAME START
    while(moveOutput != checkMate) {    //TODO BO MIGLIORABILE   
        if(turn) { //player turn 
            system("cls"); //TODO METTERE LA COSA DEL SYSTEM OS FOSAFSJAFSA
            b.printBoard();

            if(botRequestedDraw) {
                logger.log(bot.get_name(), "Asking for a draw...");
                cout << bot.get_name() << " ha richiesto la patta, vuoi accettarla? [Y/N] : ";
                cin >> input;
                if(input == "Y" || input == "y") {
                    moveOutput = drawGameOver;
                    logger.log(p1, "Draw accepted.");
                } 
                else{
                    botRequestedDraw = false;
                    logger.log(p1, "Draw declined.");
                }
            }

            cout << "\n\n" << "Last move: " << to_log;    
            do {
                cout << "\n\nINSERT MOVE : ";
                getline(cin, input);
                if(input == drawCondition) {
                    userRequestedDraw = true;
                    logger.log(p1, " requested draw");
                    break; //exit from do while
                }
                regex_search(input, coordinates, input_filter);

            } while(!regex_match(input, input_filter));

            short *mosse = conversion(coordinates);
            start.first = *(mosse + 0);
            start.second = *(mosse + 1);
            end.first = *(mosse + 2);
            end.second = *(mosse + 3);

            moveOutput = b.move(start, end, turn, false, false);
            

            while(!moveOutput.first) { //invalid move, asking for draw, king still in check, ....
                int what = moveOutput.second;
                switch (what) {
                    case 3 : {
                        logger.log(console, "Threefold draw has been requested");
                        cout << "Draw for threefold repetition rule, accept it? [Y/N] : ";
                        cin >> input;
                        if(input == "Y" || input == "y") {
                            logger.log(p1, "Draw accepted.");
                            logger.log(console, "Draw, game is ended");
                            logger.log(console, "Ending log session");
                            return;
                        }
                        else{
                            logger.log(p1, "Draw declined."); 
                            moveOutput = b.move(start, end, turn, false, true);
                        }
                        break;
                    }

                    case 4 : {  
                        cout << "The game ended with a draw!";
                        logger.log(console, "Draw, game is ended");
                        logger.log(console, "Ending log session");
                        return;
                    }

                    default: { 
                        do {
                            cout << "\n\nINSERT MOVE : ";
                            getline(cin, input);
                            if(input == drawCondition) { //TODO CHECK THIS
                                userRequestedDraw = true;
                                logger.log(p1, " requested draw");
                                break; //exit from do while
                            }
                            regex_search(input, coordinates, input_filter);

                        } while(!regex_match(input, input_filter));

                        short *mosse = conversion(coordinates);
                        start.first = *(mosse + 0);
                        start.second = *(mosse + 1);
                        end.first = *(mosse + 2);
                        end.second = *(mosse + 3);

                        moveOutput = b.move(start, end, turn, false, false);             
                    }   
                }    
            }
            to_log = "Moving " + (char) start.first + (char) start.second + writeTo + (char) end.first + (char) end.second;
            logger.log(p1, to_log);
            if(moveOutput == promotionCondition) {
                    bool promotionRes = false;
                    string pedinaDaPromuovere;
                    do {
                        cout << "\n\nA pawn can be promoted, select a new piece : ";
                        getline(cin, pedinaDaPromuovere);
                        promotionRes = b.promotion(pedinaDaPromuovere.front(), turn).first;
                        cout << promotionRes << "\n" << pedinaDaPromuovere << "\n";
                    } while (!promotionRes);
                        logger.log(p1, "Promoted pawn to " + pedinaDaPromuovere.front());
                }
            turn = !turn;
        }   
        else {//pc turn
            cout << '\n' << "Last move: " << to_log;
            if(!userRequestedDraw) {
                botRequestedDraw = bot.requestDraw();
                if(!botRequestedDraw){
                    while(moveOutput == invalidMove) {
                        botMove = bot.generateRandomMove();
                        start = botMove.first;
                        end = botMove.second;
                        moveOutput = b.move(start, end, turn, false, false);

                        if(moveOutput == promotionCondition){
                            promotionChar = bot.handlePromotion();
                            b.promotion(promotionChar, turn);
                        }
                }
                    to_log = "Moving " + (char)start.first + (char)start.second + writeTo + (char)end.first + (char)end.second;
                    logger.log(bot.get_name(), to_log);
            }
            }else botAcceptedDraw=bot.handledraw();

            if(botAcceptedDraw) {
                logger.log(bot.get_name(), "Accepted the draw");
                moveOutput = drawGameOver;
            }
        }  
    }
    logger.log(console, "The game is ended");
    logger.log(console, "Ending log session");
    return;
    //METTERE UN IF PER VERIFICARE IL COLORE DELLE PEDINE CHE CHIAMANO IL MOVE PER NON MUOVERE PEZZI AVVERSAR//proponga
}
    
void computersGame() {
    board b;
    string to_log;
    string writeTo = " to ";
    bool turn;
    pair<bool, int> moveOutput;

    //executed move
    pair <bool, int> succesfulMove = make_pair(true, 1);
    pair <bool, int> promotionCondition = make_pair(true, 2);
    pair <bool, int> checkMate = make_pair(true, 3);
    //move failed
    pair <bool, int> invalidMove = make_pair(false, 1);
    pair <bool, int> stillInCheck = make_pair(false, 2);
    pair <bool, int> threeFoldDrawCondition = make_pair(false, 3);
    pair <bool, int> drawGameOver = make_pair(false, 4);

    pair<coords, coords> botMove;

    logger logger;
    coords start;
    coords end;
    char promotionChar;
    bool bot1requestedDraw = false;
    bool bot1acceptedDraw = false;
    bool bot2requestedDraw = false;
    bool bot2acceptedDraw = false;


    string bot1name = randomName();
    bot bot1(bot1name, false, b);
    string bot2name = randomName();
    while(bot1name == bot2name){
        bot2name = randomName();
    }
    bot bot2(bot2name, false, b);


    logger.log(console, "Initializing player 1 " + bot1.get_name());
    logger.log(console, "Initialising player 2 " + bot2.get_name());

    int starter = rand() % 2;
    if(starter = 1) turn = false;
    else turn = true;

    logger.log(console, "Starting match");

    while (moveOutput != checkMate) {
        if ((bot1requestedDraw == bot2acceptedDraw) || (bot2requestedDraw == bot2acceptedDraw)) {
            moveOutput = drawGameOver;
        }
        turn = !turn;
        if (turn) {
            system("cls");
            b.printBoard();
            if (moveOutput == drawGameOver) {
                cout << "The game ended with a draw!";
                logger.log(console, "Draw, game is ended");
                logger.log(console, "Ending log session");
                return;
            }
            if (bot2requestedDraw) {
                bot1acceptedDraw = bot1.handledraw();
                if (bot2acceptedDraw == bot1requestedDraw) {
                    moveOutput == drawGameOver;
                    logger.log(bot1.get_name(), "accepted the draw!");
                } else {
                    bot1requestedDraw = bot1.requestDraw();
                    if (bot1requestedDraw == false) {
                        while (moveOutput == invalidMove) {

                            botMove = bot1.generateRandomMove();

                            start = botMove.first;
                            end = botMove.second;

                            moveOutput = b.move(start, end, turn, false, false);

                            if (moveOutput == promotionCondition) {
                                promotionChar = bot1.handlePromotion();
                                b.promotion(promotionChar, turn);
                            }
                        }

                        to_log = &"Moving "[(char) start.first] + (char) start.second + writeTo + (char) end.first +
                                 (char) end.second;
                        logger.log(bot1.get_name(), to_log);
                    } else

                        logger.log(bot1.get_name(), "requested draw");
                }


                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            } else {
                system("cls");
                b.printBoard();
                if (bot1requestedDraw) {
                    bot2acceptedDraw = bot2.handledraw();
                    if (bot2acceptedDraw == bot1requestedDraw) {
                        moveOutput == drawGameOver;
                        logger.log(bot2.get_name(), "accepted the draw!");
                    }
                } else {

                    bot2requestedDraw = bot2.requestDraw();
                    if (bot2requestedDraw == false) {
                        while (moveOutput == invalidMove) {

                            botMove = bot2.generateRandomMove();

                            start = botMove.first;
                            end = botMove.second;

                            moveOutput = b.move(start, end, turn, false, false);

                            if (moveOutput == promotionCondition) {
                                promotionChar = bot1.handlePromotion();
                                b.promotion(promotionChar, turn);
                            }

                        }
                        to_log = &"Moving "[(char) start.first] + (char) start.second + writeTo + (char) end.first +
                                 (char) end.second;
                        logger.log(bot2.get_name(), to_log);
                    } else
                        logger.log(bot2.get_name(), "requested draw");

                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
            }
        }
        logger.log(console, "The game is ended");
        logger.log(console, "Ending log session");
        return;
    }
}

int main(int argc, char *argv[]) {

    if (argc not_eq 2) {
        cout << "Please enter a valid argument!";
        return 1;
    }

    srand(time(NULL));
    bool start = false;
    string enter;
    cout << "  ______  __    __   _______      _______.     _______. \n" << 
            " /      ||  |  |  | |   ____|    /       |    /       | \n" << 
            "|  ,----'|  |__|  | |  |__      |   (----`   |   (----` \n" << 
            "|  |     |   __   | |   __|      \\   \\        \\   \\ \n" << 
            "|  `----.|  |  |  | |  |____ .----)   |   .----)   |    \n" << 
            " \\______||__|  |__| |_______||_______/    |_______/    \n" <<
            "           Starting the game, please wait....             ";


            
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    enter = argv[1];
        if(enter == "cc")
        {
            system("cls");
            computersGame();
            start = true;
        }    
        else if(enter == "pc")
        {
            system("cls");
            playerGame();
            start = true;
        }
        else cout << "Invalid selection for [" << enter << "], please provide a valid argument.\n";

        return 0;
}