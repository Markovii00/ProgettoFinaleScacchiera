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
    cout << "Enter a username : ";
    cin >> p1;
    cin.ignore();

    /*int starter = rand() % 2;
    if(starter = 1) turn = false; //bot will make first move
    else turn = true; //player will make first move
     */

    turn = false;

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
            system("clear");
            b.printBoard();

            if (!lastMove.empty()) {
                cout << "\nLast Move: " << lastMove;
            }

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


            moveOutput = b.move(start, end, turn, false, false);
            system("clear");
            if (moveOutput.first) {
                switch (moveOutput.second) {

                    // CAso 1: mossa eseguita con successo, null'altro accade
                    case 1 : {
                        logger.log(p1, "Moving \"" + input + "\"");
                        lastMove = input;
                        turn = !turn;
                        break;
                    }

                        //Caso 2: Mossa eseguita, richiesta una promozione sulla pedina mossa
                    case 2 : {
                        logger.log(p1, "Moving \"" + input + "\"");
                        lastMove = input;

                        pair<bool, bool> promotionRes = make_pair(false, false);
                        string pedinaDaPromuovere;
                        do {
                            getline(cin, pedinaDaPromuovere);
                            promotionRes = b.promotion(pedinaDaPromuovere.front(), turn);
                        } while (!promotionRes.first);

                        logger.log(p1, "Promoted to \"" + pedinaDaPromuovere + "\"");

                        if (!promotionRes.second) {
                            system("clear");
                            isGameFinished = true;
                            b.printBoard();



                            // Logger part
                            logger.log(p1, bot.get_name() + " king in Mate!");

                            // Output Part
                            cout << "\n" << bot.get_name() << " king in Mate!\nGame over!";
                        }
                        break;
                    }

                        //Caso 3: Enemy king in mate after move, game ends
                    case 3: {
                        system("clear");
                        b.printBoard();

                        isGameFinished = true;


                        // Logger part
                        logger.log(p1, bot.get_name() + " king in Mate!");

                        // Output Part
                        cout << "\n" << bot.get_name() << " king in Mate!\n";

                        break;
                    }
                }
            } else {
                switch (moveOutput.second) {


                    case 1 : {
                        cout << "\n\nInvalid move, try again!\n";
                        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                        break;
                    }


                    case 2 : {
                        cout << "\n\nYour king is still in check! Try another move";
                        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                        break;
                    }


                    case 3 : {
                        logger.log(console, "\nThreefold draw has been requested");
                        cout << "\n\nDraw for threefold repetition rule, accept it? [Y/N] : ";
                        lastMove = input;

                        do {
                            getline(cin, input);

                            if (tolower(input.front()) == 'y' || tolower(input.front()) == 'n')
                                break;
                        } while (true);

                        if(tolower(input.front()) == 'y') {
                            logger.log(p1, "Draw accepted.");
                            logger.log(console, "Draw, game is ended");
                            logger.log(console, "Ending log session");

                            cout << "\n\n Draw accepted!";

                            isGameFinished = true;
                            break;
                        }
                        else if(tolower(input.front()) == 'n'){
                            logger.log(p1, "Draw declined.");
                            b.move(start, end, turn, false, true);
                            logger.log(p1, "Moving \"" + input + "\"");
                            logger.log(p1, "Moved bypassing draw");

                            system("clear");
                            b.printBoard();
                            cout << "\n\n Draw declined! Game goes on";
                            turn = !turn;
                        }
                        break;
                    }

                    case 4: {
                        system("clear");
                        b.printBoard();

                        isGameFinished = true;


                        // Logger part
                        logger.log(p1, "Tie forced for chessman number, game over");

                        // Output Part
                        cout << "\n Tie forced for chessman numer, game end!\n";

                        break;
                    }

                    case 5: {
                        system("clear");
                        b.printBoard();

                        isGameFinished = true;


                        // Logger part
                        logger.log(p1, "Tie forced for stalled game, game over");

                        // Output Part
                        cout << "\nTie forced for stalled game\nGame Over\"\n";

                        break;
                    }

                    case 6: {
                        system("clear");
                        b.printBoard();

                        isGameFinished = true;


                        // Logger part
                        logger.log(p1, "Tie forced for 50 moves rule, game over");

                        // Output Part
                        cout << "\nTie forced for 50 move rule\nGame Over\"\n";

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
                        logger.log(bot.get_name(), "Moving \"" + input + "\"");
                        lastMove = conv_readable(botMove);
                        turn = !turn;
                        break;
                    }

                        //Caso 2: Mossa eseguita, richiesta una promozione sulla pedina mossa
                    case 2 : {
                        logger.log(bot.get_name(), "Moving \"" + input + "\"");
                        lastMove = conv_readable(botMove);

                        pair<bool, bool> promotionRes = make_pair(false, false);
                        string pedinaDaPromuovere = bot.handlePromotion();
                        promotionRes = b.promotion(pedinaDaPromuovere.front(), turn);

                        logger.log(bot.get_name(), "Promoted to \"" + pedinaDaPromuovere + "\"");

                        if (!promotionRes.second) {
                            isGameFinished = true;

                            system("clear");
                            b.printBoard();
                            // Logger part
                            logger.log(bot.get_name(), p1 + " king in Mate!");

                            // Output Part
                            cout << "\n"<<p1<<" King in Mate!\n";

                            break;
                        }
                    }

                        //Caso 3: Enemy king in mate after move, game ends
                    case 3: {
                        system("clear");
                        b.printBoard();

                        isGameFinished = true;


                        // Logger part
                        logger.log(bot.get_name(), p1 + " king in Mate!");

                        // Output Part
                        cout << "\n"<<p1<<" King in Mate!\n";

                        break;
                    }
                }
            } else {
                switch (moveOutput.second) {


                    case 1 : {
                        cout << "\n\nInvalid move, try again!";
                        break;
                    }


                    case 2 : {
                        cout << "\n\nYour king is still in check! Try another move";
                        break;
                    }


                    case 3 : {
                        logger.log(console, "Threefold draw has been requested");
                        lastMove = conv_readable(botMove);

                        if(bot.handledraw()) {
                            logger.log(bot.get_name(), "Draw accepted.");
                            logger.log(console, "Draw, game is ended");
                            logger.log(console, "Ending log session");

                            system("clear");
                            b.printBoard();
                            cout << "\n\nThreefold draw has been requested and bot accepted draw!";

                            isGameFinished = true;
                            break;
                        }
                        else {
                            logger.log(bot.get_name(), "Draw declined.");
                            b.move(start, end, turn, false, true);
                            logger.log(bot.get_name(), "Moving \"" + input + "\"");
                            logger.log(bot.get_name(), "Moved bypassing draw");
                            system("clear");
                            b.printBoard();
                            cout << "\n\n Threefold draw has been requested and botdeclined, game goes on";
                            turn = !turn;
                        }
                        break;
                    }

                    case 4: {
                        system("clear");
                        b.printBoard();

                        isGameFinished = true;


                        // Logger part
                        logger.log(bot.get_name(), "Tie forced for chessman number, game over");

                        // Output Part
                        cout << "\n Tie forced for chessman numer, game end!\n";

                        break;
                    }

                    case 5: {
                        system("clear");
                        b.printBoard();

                        isGameFinished = true;


                        // Logger part
                        logger.log(bot.get_name(), "Tie forced for stalled game, game over");

                        // Output Part
                        cout << "\nTie forced for stalled game\nGame Over\"\n";

                        break;
                    }

                    case 6: {
                        system("clear");
                        b.printBoard();

                        isGameFinished = true;


                        // Logger part
                        logger.log(bot.get_name(), "Tie forced for 50 moves rule, game over");

                        // Output Part
                        cout << "\nTie forced for 50 move rule\nGame Over\"\n";

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
    string to_log;
    string writeTo = " to ";
    bool turn;
    pair<bool, int> moveOutput;

    pair<coords, coords> botMove;


    logger logger;
    coords start;
    coords end;

    string bot1name = randomName();
    bot bot1(bot1name, false, b);
    string bot2name = randomName();
    while(bot1name == bot2name){
        bot2name = randomName();
    }
    bot bot2(bot2name, true, b);


    logger.log(console, "Initializing player 1 " + bot1.get_name());
    logger.log(console, "Initialising player 2 " + bot2.get_name());

    int starter = rand() % 2;
    if(starter = 1) turn = false;
    else turn = true;

    logger.log(console, "Starting match");

    bool isGameFinished = false;
    string lastMove;
    string input;

    while (!isGameFinished) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        system("clear");
        if(!turn){
            cout << "\n --- Moves "<<bot1.get_name()<<" ---\n";

            botMove =  bot1.generateRandomMove();

            start = botMove.first;
            end = botMove.second;

            input = start.first + start.second + end.first + end.second;

            moveOutput = b.move(start, end, turn, false, false);

            if (moveOutput.first) {
                switch (moveOutput.second) {

                    // CAso 1: mossa eseguita con successo, null'altro accade
                    case 1 : {
                        b.printBoard();

                        if (!lastMove.empty()) {
                            cout << "\nLast Move: " << lastMove;
                        }

                        logger.log(bot1.get_name(), "Moving \"" + input + "\"");
                        lastMove = conv_readable(botMove);
                        turn = !turn;
                        break;
                    }

                        //Caso 2: Mossa eseguita, richiesta una promozione sulla pedina mossa
                    case 2 : {
                        logger.log(bot1.get_name(), "Moving \"" + input + "\"");

                        lastMove = conv_readable(botMove);

                        pair<bool, bool> promotionRes = make_pair(false, false);
                        string pedinaDaPromuovere = bot1.handlePromotion();
                        promotionRes = b.promotion(pedinaDaPromuovere.front(), turn);

                        lastMove = conv_readable(botMove);
                        cout << "\n\nPromoted to " << pedinaDaPromuovere << "\n";
                        logger.log(bot1.get_name(), "Promoted to \"" + pedinaDaPromuovere + "\"");

                        if (!promotionRes.second) {
                            system("clear");
                            b.printBoard();

                            if (!lastMove.empty()) {
                                cout << "\nLast Move: " << lastMove;
                            }

                            isGameFinished = true;


                            // Logger part
                            logger.log(bot1.get_name(), "Enemy king in Mate!");

                            // Output Part
                            cout << bot2.get_name() <<"\n King in Mate!\n";

                        }
                        break;
                    }

                        //Caso 3: Enemy king in mate after move, game ends
                    case 3: {
                        system("clear");
                        b.printBoard();

                        if (!lastMove.empty()) {
                            cout << "\nLast Move: " << lastMove;
                        }

                        isGameFinished = true;


                        // Logger part
                        logger.log(bot1.get_name(), "Enemy king in Mate!");

                        // Output Part
                        cout << "\n"<<bot2.get_name()<<" king in Mate!\n";

                        break;
                    }
                }
            } else {
                switch (moveOutput.second) {


                    case 1 : {
                        cout << "\n\nInvalid move, try again!";
                        break;
                    }


                    case 2 : {
                        cout << "\n\nYour king is still in check! Try another move";
                        break;
                    }


                    case 3 : {
                        logger.log(console, "Threefold draw has been requested");
                        lastMove = input;

                        if(bot2.handledraw()) {
                            logger.log(bot1.get_name(), "Draw accepted.");
                            logger.log(console, "Draw, game is ended");
                            logger.log(console, "Ending log session");

                            system("clear");
                            b.printBoard();

                            if (!lastMove.empty()) {
                                cout << "\nLast Move: " << lastMove;
                            }
                            cout << "\n\n"<<bot1.get_name()<<"accepted Threefold draw!";

                            isGameFinished = true;
                            break;
                        }
                        else {
                            logger.log(bot1.get_name(), "Draw declined.");
                            b.move(start, end, turn, false, true);
                            logger.log(bot1.get_name(), "Moving \"" + input + "\"");
                            logger.log(bot1.get_name(), "Moved bypassing draw");

                            system("clear");
                            b.printBoard();

                            if (!lastMove.empty()) {
                                cout << "\nLast Move: " << lastMove;
                            }

                            lastMove = conv_readable(botMove);
                            cout << "\n\n "<< bot1.get_name()<<" declined Threefold draw! Game goes on";
                            turn = !turn;
                        }
                        break;
                    }

                    case 4: {
                        system("clear");

                        b.printBoard();

                        if (!lastMove.empty()) {
                            cout << "\nLast Move: " << lastMove;
                        }

                        isGameFinished = true;


                        // Logger part
                        logger.log(bot1.get_name(), "Tie forced for chessman number, game over");

                        // Output Part
                        cout << "\n Tie forced for chessman numer, game end!\n";

                        break;
                    }

                    case 5: {
                        system("clear");

                        b.printBoard();

                        if (!lastMove.empty()) {
                            cout << "\nLast Move: " << lastMove;
                        }

                        isGameFinished = true;


                        // Logger part
                        logger.log(bot2.get_name(), "Tie forced for stalled game, game over");

                        // Output Part
                        cout << "\nTie forced for stalled game\nGame Over\"\n";

                        break;
                    }

                    case 6: {
                        system("clear");

                        b.printBoard();

                        if (!lastMove.empty()) {
                            cout << "\nLast Move: " << lastMove;
                        }

                        isGameFinished = true;


                        // Logger part
                        logger.log(bot1.get_name(), "Tie forced for 50 moves rule, game over");

                        // Output Part
                        cout << "\nTie forced for 50 move rule\nGame Over\"\n";

                        break;
                    }
                }
            }
        }
        else{
            cout << "\n --- Moves "<<bot2.get_name()<<" ---\n";

            botMove =  bot2.generateRandomMove();

            start = botMove.first;
            end = botMove.second;

            input = start.first + start.second + end.first + end.second;

            moveOutput = b.move(start, end, turn, false, false);

            if (moveOutput.first) {
                switch (moveOutput.second) {

                    // CAso 1: mossa eseguita con successo, null'altro accade
                    case 1 : {
                        logger.log(bot2.get_name(), "Moving \"" + input + "\"");

                        b.printBoard();

                        if (!lastMove.empty()) {
                            cout << "\nLast Move: " << lastMove;
                        }

                        lastMove = conv_readable(botMove);
                        turn = !turn;

                        break;
                    }

                        //Caso 2: Mossa eseguita, richiesta una promozione sulla pedina mossa
                    case 2 : {
                        logger.log(bot2.get_name(), "Moving \"" + input + "\"");

                        lastMove = conv_readable(botMove);
                        pair<bool, bool> promotionRes = make_pair(false, false);

                        b.printBoard();

                        if (!lastMove.empty()) {
                            cout << "\nLast Move: " << lastMove;
                        }

                        string pedinaDaPromuovere = bot2.handlePromotion();
                        promotionRes = b.promotion(pedinaDaPromuovere.front(), turn);

                        logger.log(bot2.get_name(), "Promoted to \"" + pedinaDaPromuovere + "\"");
                        cout << "\n\nPromoted to " << pedinaDaPromuovere << "\n";

                        if (!promotionRes.second) {
                            b.printBoard();
                            isGameFinished = true;


                            // Logger part
                            logger.log(bot2.get_name(), "Enemy king in Mate!");

                            // Output Part
                            cout << bot1.get_name() << "\n King in Mate!\n";
                        }
                        break;
                    }

                        //Caso 3: Enemy king in mate after move, game ends
                    case 3: {
                        system("clear");
                        b.printBoard();
                        b.printBoard();

                        isGameFinished = true;

                        if (!lastMove.empty()) {
                            cout << "\nLast Move: " << lastMove;
                        }

                        // Logger part
                        logger.log(bot2.get_name(), bot1.get_name() + " king in Mate!");

                        // Output Part
                        cout << bot1.get_name() << " King in Mate!\n";

                        break;
                    }
                }
            } else {
                switch (moveOutput.second) {


                    case 1 : {
                        cout << "\n\nInvalid move, try again!";
                        break;
                    }


                    case 2 : {
                        cout << "\n\nYour king is still in check! Try another move";
                        break;
                    }


                    case 3 : {
                        logger.log(console, "Threefold draw has been requested");
                        lastMove = input;

                        if(bot2.handledraw()) {
                            logger.log(bot2.get_name(), "Draw accepted.");
                            logger.log(console, "Draw, game is ended");
                            logger.log(console, "Ending log session");
                            system("clear");
                            b.printBoard();
                            cout << "\n\n "<<bot2.get_name()<<" accepted Threefold draw!";

                            isGameFinished = true;
                            break;
                        }
                        else {
                            logger.log(bot2.get_name(), "Draw declined.");
                            b.move(start, end, turn, false, true);
                            logger.log(bot2.get_name(), "Moving \"" + input + "\"");
                            logger.log(bot2.get_name(), "Moved bypassing draw");

                            system("clear");
                            b.printBoard();

                            if (!lastMove.empty()) {
                                cout << "\nLast Move: " << lastMove;
                            }
                            cout << "\n\n "<<bot2.get_name()<<" declined Threefold draw!";

                            lastMove = conv_readable(botMove);
                            turn = !turn;
                        }
                        break;
                    }

                    case 4: {
                        system("clear");
                        b.printBoard();

                        if (!lastMove.empty()) {
                            cout << "\nLast Move: " << lastMove;
                        }

                        isGameFinished = true;


                        // Logger part
                        logger.log(bot2.get_name(), "Tie forced for chessman number, game over");

                        // Output Part
                        cout << "\n Tie forced for chessman numer, game end!\n";

                        break;
                    }

                    case 5: {
                        system("clear");
                        b.printBoard();

                        if (!lastMove.empty()) {
                            cout << "\nLast Move: " << lastMove;
                        }

                        isGameFinished = true;


                        // Logger part
                        logger.log(bot2.get_name(), "Tie forced for stalled game, game over");

                        // Output Part
                        cout << "\nTie forced for stalled game\nGame Over\"\n";

                        break;
                    }

                    case 6: {
                        system("clear");
                        b.printBoard();

                        if (!lastMove.empty()) {
                            cout << "\nLast Move: " << lastMove;
                        }

                        isGameFinished = true;


                        // Logger part
                        logger.log(bot2.get_name(), "Tie forced for 50 moves rule, game over");

                        // Output Part
                        cout << "\nTie forced for 50 move rule\nGame Over\"\n";

                        break;
                    }
                }
            }
        }
    }

    logger.log(console, "The game is ended");
    logger.log(console, "Ending log session");
    b.printBoard();
    return;
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