/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#include "board.h"
#include "include/log/logger.hpp"
#include "bot.h"
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

string conv_readable(pair<coords, coords> move) {
    string to_ret = "";
    switch (move.first.second) {

        case 0: {
            to_ret += "A";
            break;
        }
        case 1: {
            to_ret += "B";
            break;
        }
        case 2: {
            to_ret += "C";
            break;
        }
        case 3: {
            to_ret += "D";
            break;
        }
        case 4: {
            to_ret += "E";
            break;
        }
        case 5: {
            to_ret += "F";
            break;
        }
        case 6: {
            to_ret += "G";
            break;
        }
        case 7: {
            to_ret += "H";
            break;
        }
    }
    to_ret += to_string(8-move.first.first);
    to_ret += " ";
    switch (move.second.second) {

        case 0: {
            to_ret += "A";
            break;
        }
        case 1: {
            to_ret += "B";
            break;
        }
        case 2: {
            to_ret += "C";
            break;
        }
        case 3: {
            to_ret += "D";
            break;
        }
        case 4: {
            to_ret += "E";
            break;
        }
        case 5: {
            to_ret += "F";
            break;
        }
        case 6: {
            to_ret += "G";
            break;
        }
        case 7: {
            to_ret += "H";
            break;
        }

    }
    to_ret += to_string(8-move.second.first);

    return to_ret;
}

void clear_output() {
    system("clear");
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

    vector<char> promotionSet{'d', 'a', 't', 'c'};


    string drawCondition = "FF";
    string to_log = " ";
    string writeTo = " to ";


    //STARTING THE PROGRAM
    logger.log(console, "Welcome");
    logger.log(console, "Starting new log session");

    string p1;
    cout << "\n\n\nEnter a username : ";
    cin >> p1;
    cin.ignore();

    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> starter(0, 1);

    bool set_moving = false;
    if(starter(engine) == 1) {
        turn = false; //bot will make first move
    }
    else {
        turn = true; //player will make first move
    }

    string botname = randomName();

    bot bot(botname, true, b);

    logger.log(console, "Initializing player 1 \"" + p1 + "\"");
    logger.log(console, "Initializing player 2 \"" + bot.get_name() + "\"");

    logger.log(console, "Starting match");

    //GAME START
    bool isGameFinished = false;
    string lastMove{};

    while (!isGameFinished) {
        //PLayer turn
        if (!turn) {
            clear_output();
            b.printBoard();
            if (!lastMove.empty())
                cout << "\nLast Move: " << lastMove;

            // get player move
            regex input_filter("^([a-hA-H]){1}([1-8]){1} ([a-hA-H]){1}([1-8]){1}$");
            smatch coordinates;
            do {
                cout << "\n\nINSERT MOVE : ";
                getline(cin, input);
                regex_search(input, coordinates, input_filter);
            }while(!regex_match(input, input_filter));

            short *mosse= conversion(coordinates);
            start.first = *(mosse + 0);
            start.second = *(mosse + 1);
            end.first = *(mosse + 2);
            end.second = *(mosse + 3);


            moveOutput = b.move(start, end, set_moving, false, false);

            if (moveOutput.first) {
                switch (moveOutput.second) {

                    // Caso 1: mossa eseguita con successo, null'altro accade
                    case 1 : {
                        logger.log(p1, "Moving \"" + input + "\"");
                        lastMove = input;
                        set_moving = !set_moving;
                        turn = !turn;
                        break;
                    }

                        //Caso 2: Mossa eseguita, richiesta una promozione sulla pedina mossa
                    case 2 : {
                        logger.log(p1, "Moving \"" + input + "\"");
                        lastMove = input;

                        pair<bool, bool> promotionRes;
                        string pedinaDaPromuovere;
                        cout << "\n\nChose a chessman for your promotion -> ";
                        do {
                            getline(cin, pedinaDaPromuovere);
                            promotionRes = b.promotion(pedinaDaPromuovere.front(), set_moving);
                        } while (!promotionRes.first);

                        logger.log(p1 , "Promoting a pawn to \"" + pedinaDaPromuovere + "\"");
                        cout << "\n\n Promoting to " << pedinaDaPromuovere << "\n";

                        if (!promotionRes.second) {
                            clear_output();
                            b.printBoard();
                            if (!lastMove.empty())
                                cout << "\nLast Move: " << lastMove;

                            isGameFinished = true;

                            // Logger part
                            logger.log(p1, bot.get_name() + " king in Mate");


                            // Output Part
                            cout << "\n Promotion putted " << bot.get_name() << "'s king in mate!\nGame Over!";
                        }
                        lastMove = input;
                        turn = !turn;
                        set_moving = !set_moving;
                        break;
                    }

                        //Caso 3: Enemy king in mate after move, game ends
                    case 3: {
                        clear_output();
                        b.printBoard();
                        if (!lastMove.empty())
                            cout << "\nLast Move: " << lastMove;

                        logger.log(p1, "Moving \"" + input + "\"");

                        cout << "\n\nYou putted " << bot.get_name() << "'s king in mate!\nGame Over!";
                        isGameFinished = true;
                        logger.log(p1, "Enemy king in Mate!");
                        break;
                    }
                }
            } else {
                switch (moveOutput.second) {


                    case 1 : {
                        clear_output();
                        cout << "\n\nInvalid move, try again!\n";
                        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                        break;
                    }


                    case 2 : {
                        clear_output();
                        cout << "\n\nYour king is still in check! Try another move";
                        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                        break;
                    }


                    case 3 : {
                        logger.log(console, "\nThreefold draw has been requested");
                        cout << "\n\nDraw for threefold repetition rule, accept it? [Y/N] : ";
                        do {
                            getline(cin, input);

                            if (tolower(input.front()) == 'y' || tolower(input.front()) == 'n')
                                break;
                        } while (true);

                        if(tolower(input.front()) == 'y') {
                            logger.log(p1, "Draw accepted.");
                            logger.log(console, "Ending game...");

                            clear_output();
                            b.printBoard();
                            if (!lastMove.empty())
                                cout << "\nLast Move: " << lastMove;

                            cout << "\n\nDraw accepted!\nGame Over!";

                            isGameFinished = true;

                            break;
                        }
                        else if(tolower(input.front()) == 'n'){
                            logger.log(p1, "Draw declined.");

                            b.move(start, end, set_moving, false, true);
                            logger.log(p1, "Moving \"" + input + "\"");
                            logger.log(p1, "Moved bypassing draw");

                            clear_output();
                            b.printBoard();
                            if (!lastMove.empty())
                                cout << "\nLast Move: " << lastMove;

                            cout << "\n\n Draw declined! Game goes on";
                            lastMove = input;
                            turn = !turn;
                            set_moving = !set_moving;
                        }
                        break;
                    }

                    case 4: {
                        clear_output();
                        b.printBoard();
                        if (!lastMove.empty())
                            cout << "\nLast Move: " << lastMove;

                        isGameFinished = true;

                        // Logger part
                        logger.log(p1, "Tie forced due to chessman number, ending game...");

                        // Output Part
                        cout << "\n Tie forced for chessman number!\nGame Over!\n";

                        break;
                    }

                    case 5: {
                        clear_output();
                        b.printBoard();
                        if (!lastMove.empty())
                            cout << "\nLast Move: " << lastMove;

                        isGameFinished = true;


                        // Logger part
                        logger.log(p1, "Tie forced for stalled game, ending game...");

                        // Output Part
                        cout << "\nTie forced for stalled game\nGame Over!";

                        break;
                    }

                    case 6: {
                        clear_output();
                        b.printBoard();
                        if (!lastMove.empty())
                            cout << "\nLast Move: " << lastMove;

                        isGameFinished = true;

                        logger.log(p1, "Tie forced for 50 moves rule, ending game...");

                        cout << "\nTie forced for 50 move rule\nGame Over!\n";

                        break;
                    }
                }
            }
        }
        else{
            botMove =  bot.generateRandomMove();

            start = botMove.first;
            end = botMove.second;

            moveOutput = b.move(start, end, turn, false, false);

            if (moveOutput.first) {
                switch (moveOutput.second) {

                    // Caso 1: mossa eseguita con successo, null'altro accade
                    case 1 : {
                        logger.log(bot.get_name(), "Moving \"" + conv_readable(botMove) + "\"");
                        lastMove = conv_readable(botMove);

                        turn = !turn;
                        set_moving = !set_moving;
                        break;
                    }

                        //Caso 2: Mossa eseguita, richiesta una promozione sulla pedina mossa
                    case 2 : {
                        logger.log(bot.get_name(), "Moving \"" + conv_readable(botMove) + "\"");
                        lastMove = conv_readable(botMove);

                        pair<bool, bool> promotionRes;
                        string pedinaDaPromuovere = bot.handlePromotion();
                        promotionRes = b.promotion(pedinaDaPromuovere.front(), turn);

                        logger.log(bot.get_name(), "Promoting a pawn to \"" + pedinaDaPromuovere + "\"");

                        if (!promotionRes.second) {
                            isGameFinished = true;

                            clear_output();
                            b.printBoard();
                            if (!lastMove.empty())
                                cout << "\nLast move: " << lastMove << "\n";

                            cout << "\n"<<bot.get_name()<<" promotion putted your king in mate!\nGame Over!";
                            // Logger part
                            logger.log(bot.get_name(), "Enemy king in Mate");
                            logger.log(bot.get_name(), "Ending game...");
                            break;
                        }

                        turn = !turn;
                        set_moving = !set_moving;
                    }

                        //Caso 3: Enemy king in mate after move, game ends
                    case 3: {
                        clear_output();
                        b.printBoard();
                        if (!lastMove.empty())
                            cout << "\nLast move: " << lastMove << "\n";

                        isGameFinished = true;


                        // Logger part
                        logger.log(bot.get_name(), "Enemy king in Mate");
                        logger.log(bot.get_name(), "Ending game...");

                        // Output Part
                        cout << "\n"<<bot.get_name()<<" put your king in mate!\nGame Over!";

                        break;
                    }
                }
            }
            else {
                switch (moveOutput.second) {

                    case 3 : {

                        logger.log(console, "Threefold draw has been requested");

                        if(bot.handledraw()) {
                            logger.log(bot.get_name(), "Draw accepted.");
                            logger.log(console, "Ending game...");

                            clear_output();
                            b.printBoard();
                            if (!lastMove.empty())
                                cout << "\nLast Move: " << lastMove;

                            cout << "A Threefold Draw was asked to " << bot.get_name() << " and was accepted!\nGame Over";

                            isGameFinished = true;
                        }
                        else {
                            logger.log(bot.get_name(), "Draw declined.");
                            b.move(botMove.first, botMove.second, set_moving, false, true);


                            lastMove = conv_readable(botMove);

                            logger.log(bot.get_name(), "Moving \"" + lastMove + "\"");
                            logger.log(bot.get_name(), "Moved bypassing draw");

                            cout << "\nA Threefold Draw was asked to "<< bot.get_name()<<" and was declined! Game resumed!\n\n";
                            turn = !turn;
                            set_moving = !set_moving;
                        }
                        break;
                    }

                    case 4: {
                        clear_output();
                        b.printBoard();
                        if (!lastMove.empty())
                            cout << "\nLast Move: " << lastMove;

                        isGameFinished = true;


                        // Logger part
                        logger.log(bot.get_name(), "Tie forced due to chessman number, ending game...");

                        // Output Part
                        cout << "\n Tie forced for chessman number!\nGame Over!\n";
                        break;
                    }

                    case 5: {
                        clear_output();
                        b.printBoard();
                        if (!lastMove.empty())
                            cout << "\nLast Move: " << lastMove;

                        isGameFinished = true;

                        logger.log(bot.get_name(), "Tie forced for stalled game, ending game...");

                        // Output Part
                        cout << "\nTie forced for stalled game\nGame Over!";
                        break;
                    }

                    case 6: {
                        clear_output();
                        b.printBoard();
                        if (!lastMove.empty())
                            cout << "\nLast Move: " << lastMove;

                        isGameFinished = true;

                        logger.log(bot.get_name(), "Tie forced for 50 moves rule, ending game...");

                        cout << "\nTie forced for 50 move rule\nGame Over!\n";

                        break;
                    }
                }
            }
        }
    }

    logger.log(console, "The game is ended");
    logger.log(console, "Ending log session");
}

void computersGame() {
    board b;
    bool turn = false;

    logger logstream;
    logstream.log(console, "Welcome");
    logstream.log(console, "Starting new log session");

    string bot1name = randomName();
    bot bot1(bot1name, false, b);
    string bot2name = randomName();
    while(bot1name == bot2name){
        bot2name = randomName();
    }
    bot bot2(bot2name, true, b);

    logstream.log(console, "Initializing player 1 \"" + bot1.get_name() + "\"");
    logstream.log(console, "Initializing player 2 \"" + bot2.get_name() + "\"");

    bool isGameEnded = false;

    string lastMove{};
    while (!isGameEnded) {
        // Creating move
        bot& moving_bot = (bot1.get_set() == turn) ? bot1 : bot2;
        bot& enemy_bot = (bot1.get_set() == turn) ? bot2 : bot1;

        pair<coords, coords> botMove = moving_bot.generateRandomMove();

        pair<bool, int> moveOutput = b.move(botMove.first, botMove.second, turn, false, false);

        if (moveOutput.first) {
            switch (moveOutput.second) {

                //case 1, move ok, nothing to report
                case 1: {
                    clear_output();
                    cout << "\n --- "<<moving_bot.get_name()<<"'s turn ---\n";
                    b.printBoard();
                    if (!lastMove.empty())
                        cout << "\nLast move: " << lastMove << "\n";

                    turn = !turn;
                    lastMove = conv_readable(botMove);

                    logstream.log(moving_bot.get_name(), "Moving \"" + lastMove + "\"");

                    break;
                }

                case 2 : {
                    string promotionPiece = moving_bot.handlePromotion();
                    pair<bool, bool> promotionOutput = b.promotion(promotionPiece.front(), turn);


                    clear_output();
                    cout << "\n --- "<<moving_bot.get_name()<<"'s turn ---\n";
                    b.printBoard();
                    if (!lastMove.empty())
                        cout << "\nLast move: " << lastMove << "\n";

                    lastMove = conv_readable(botMove);
                    logstream.log(moving_bot.get_name(), "Moving \"" + lastMove + "\"");
                    logstream.log(moving_bot.get_name(), "Promoting a pawn to \"" + promotionPiece + "\"");

                    cout << "\n\n Promoting to " << promotionPiece << " a pawn from " << moving_bot.get_name() << "'s set";
                    logstream.log(moving_bot.get_name(), "Moving \"" + conv_readable(botMove) + "\"");

                    if (!promotionOutput.second) {
                        cout << "\n Promotion putted " << enemy_bot.get_name() << "'s king in mate!\nGame Over!";
                        logstream.log(moving_bot.get_name(), "Enemy king in Mate");
                        logstream.log(console, "Ending game...");
                        isGameEnded = true;
                    }

                    turn = !turn;
                    break;
                }

                case 3 : {
                    clear_output();
                    cout << "\n --- "<<moving_bot.get_name()<<"'s turn ---\n";
                    b.printBoard();
                    if (!lastMove.empty())
                        cout << "\nLast move: " << lastMove << "\n";

                    cout << "\n\n" << moving_bot.get_name() <<" putted " << moving_bot.get_name() << "'s king in mate!\nGame Over!";

                    logstream.log(moving_bot.get_name(), "Moving \"" + conv_readable(botMove) + "\"");
                    logstream.log(moving_bot.get_name(), "Enemy king in Mate");
                    logstream.log(console, "Ending game...");

                    isGameEnded = true;
                    break;
                }
            }
        } else {
            switch (moveOutput.second) {

                case 3 : {
                    clear_output();
                    cout << "\n --- "<<moving_bot.get_name()<<"'s turn ---\n";
                    b.printBoard();

                    if (!lastMove.empty())
                        cout << "\nLast Move: " << lastMove;

                    cout << "\n\nAsking to "<<moving_bot.get_name()<<" for a Threefold draw!\n";

                    logstream.log(console, "Threefold draw has been requested");

                    if(bot2.handledraw()) {
                        logstream.log(moving_bot.get_name(), "Draw accepted.");
                        logstream.log(console, "Ending game...");

                        cout << moving_bot.get_name()<<"accepted Threefold draw!\n";

                        isGameEnded = true;
                    }
                    else {
                        logstream.log(moving_bot.get_name(), "Draw declined.");
                        b.move(botMove.first, botMove.second, turn, false, true);

                        clear_output();
                        cout << "\n --- "<<moving_bot.get_name()<<"'s turn ---\n";
                        b.printBoard();

                        if (!lastMove.empty())
                            cout << "\nLast Move: " << lastMove;

                        lastMove = conv_readable(botMove);

                        logstream.log(moving_bot.get_name(), "Moving \"" + lastMove + "\"");
                        logstream.log(moving_bot.get_name(), "Moved bypassing draw");

                        cout << "\n "<< bot1.get_name()<<" declined Threefold draw! Game resumed";
                        turn = !turn;
                    }
                    break;
                }

                case 4: {
                    clear_output();
                    cout << "\n --- "<<moving_bot.get_name()<<"'s turn ---\n";
                    b.printBoard();
                    if (!lastMove.empty())
                        cout << "\nLast Move: " << lastMove;

                    isGameEnded = true;


                    // Logger part
                    logstream.log(moving_bot.get_name(), "Tie forced due to chessman number, ending game...");

                    // Output Part
                    cout << "\n Tie forced for chessman number!\nGame Over!\n";
                    break;
                }

                case 5: {
                    clear_output();
                    cout << "\n --- "<<moving_bot.get_name()<<"'s turn ---\n";
                    b.printBoard();
                    if (!lastMove.empty())
                        cout << "\nLast Move: " << lastMove;

                    isGameEnded = true;

                    logstream.log(moving_bot.get_name(), "Tie forced for stalled game, ending game...");

                    // Output Part
                    cout << "\nTie forced for stalled game\nGame Over!";
                    break;
                }

                case 6: {
                    clear_output();
                    cout << "\n --- "<<moving_bot.get_name()<<"'s turn ---\n";
                    b.printBoard();
                    if (!lastMove.empty())
                        cout << "\nLast Move: " << lastMove;

                    isGameEnded = true;

                    logstream.log(moving_bot.get_name(), "Tie forced for 50 moves rule, ending game...");

                    cout << "\nTie forced for 50 move rule\nGame Over!\n";

                    break;
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(150));
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
         "           Starting the game, please wait....           \n\n";



    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    enter = argv[1];
    if(enter == "cc")
    {
        system("clear");
        computersGame();
        start = true;
    }
    else if(enter == "pc")
    {
        system("clear");
        playerGame();
        start = true;
    }
    else cout << "Invalid selection for [" << enter << "], please provide a valid argument.\n";

    return 0;
}