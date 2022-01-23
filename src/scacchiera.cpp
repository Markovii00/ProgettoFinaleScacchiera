/**
 * @author Francesco Colla - 2007968 (francesco.colla.2@studenti.unipd.it)
 */

#include "../include/board.h"
#include "../include/log/logger.h"
#include "../include/bot.h"
#include <cctype>
#include <thread>


using namespace std;
string const console = "console";

//Picks a random name from the names vector and uses it to initialize the bot
string randomName(){
    vector<string> names = {"AMOGUS", "Baymax", "BakaBot", "Wall-E", "MapBot", "SUSBot", "Bixby", "Cortana", "Alexa"};
    short namesLenght = names.size();
    short nameindex = rand() % namesLenght;
    std::string nameToSet = names.at(nameindex);

    return nameToSet;
}

//Convert the move coordinates to a readable form, used to print the last move and to save into log file 
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

//defines the os thanks to the compiler macros and sets the clear statement, used for multi-platform compatibility
void clear_output() {
    #ifdef _unix_
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif
    #endif
}

//Convert input regex in matrix coordinates
vector<short> conversion(smatch& m) {
    short fromCol = toupper(m.str(1)[0]) - 65;
    short fromRow = 8 - stoi(m.str(2));
    short toCol = toupper(m.str(3)[0]) - 65;
    short toRow = 8 - stoi(m.str(4));

    vector<short> ret{fromRow, fromCol, toRow, toCol};
    return ret;
}

//handles player vs computer
void playerGame() {
    //INIZIALIZING VARIABLES
    board b;
    coords start;
    coords end;
    pair <bool, int> moveOutput;
    pair<coords, coords> botMove;
    string input;
    bool requestDraw = false;
    bool acceptedDraw = false;
    bool turn;

    vector<char> promotionSet{'d', 'a', 't', 'c'};
    string drawCondition = "FF";

    //STARTING THE PROGRAM AND CREATING LOG FILE INITIALIZING PLAYER AND BOT
    logger logger;
    logger.log(console, "Welcome");
    logger.log(console, "Starting new log session");

    string p1;
    cout << "\n\n\nEnter a username : ";
    cin >> p1;
    cin.ignore();

    //using a random device from <random> to pick the starting side (random 0 or 1)
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<int> starter(0, 1);

    bool set_moving = false;
    if(starter(engine) == 1) {
        turn = true; //bot will make first move
        cout << "\nThe bot will start!\n";
    }
    else {
        turn = false; //player will make first move
        cout << "\nYou will start!\n";
    }

    string botname = randomName();

    bot bot(botname,!turn, b);

    logger.log(console, "Initializing player 1 \"" + p1 + "\"");
    logger.log(console, "Initializing player 2 \"" + bot.get_name() + "\"");

    logger.log(console, "Starting match");

    //ALL READY GAME STARTS
    bool isGameFinished = false;
    string lastMove{};

    while (!isGameFinished) {
        if (!turn) { //PLAYER TURN 
            clear_output();
            b.printBoard();
            if (!lastMove.empty())
                cout << "\nLast Move: " << lastMove;

            // get player move and converts to usable coordinates
            regex input_filter("^([a-hA-H]){1}([1-8]){1} ([a-hA-H]){1}([1-8]){1}$");
            smatch coordinates;
            do {
                cout << "\n\nINSERT MOVE : ";
                getline(cin, input);
                if(input == drawCondition){
                    requestDraw = true;
                    break;
                }
                regex_search(input, coordinates, input_filter);
            }while(!regex_match(input, input_filter));

            if(!requestDraw) {
                vector<short> moves = conversion(coordinates);
                start.first = moves.at(0);
                start.second = moves.at(1);
                end.first = moves.at(2);
                end.second = moves.at(3);

                //tries to execute the move and outputs a condition to handle the result see board.h
                moveOutput = b.move(start, end, set_moving, false, false);

            } else moveOutput = make_pair(false, 7); //case for requested draw
            
            //Moveoutput is true, move has been executed, handling variuos possibilities
            if (moveOutput.first) {
                switch (moveOutput.second) {

                    case 1 : {// Case 1: Executed move, no other actions required, logs the move and changes turn
                        logger.log(p1, "Moving \"" + input + "\"");
                        lastMove = input;
                        set_moving = !set_moving;
                        turn = !turn;
                        break;
                    }

                    case 2 : { //Case 2: Executed move, required promotion on the selected pawn, logs move and promotion and changes turn
                        logger.log(p1, "Moving \"" + input + "\"");
                        lastMove = input;

                        pair<bool, bool> promotionRes;
                        string pawnToPromote;
                        //Handles the promotion and runs until the input is valid
                        do {
                            cout << "\n\nChose a chessman for your promotion -> ";
                            getline(cin, pawnToPromote);
                            promotionRes = b.promotion(pawnToPromote.front(), set_moving);
                        } while (!promotionRes.first);

                        logger.log(p1 , "Promoting a pawn to \"" + pawnToPromote + "\"");
                        cout << "\n\nPromoting to " << pawnToPromote << "\n";
                        
                        if (!promotionRes.second) {//PromotionRes.second is a signal from board.cpp, if true the new piece mates the enemy king
                            clear_output();
                            b.printBoard();
                            if (!lastMove.empty())
                                cout << "\nLast Move: " << lastMove;

                            isGameFinished = true;

                            // Logger part
                            logger.log(p1, bot.get_name() + " king in Mate");
                            // Output Part
                            cout << "\nPromotion putted " << bot.get_name() << "'s king in mate!\nGame Over!\n";
                        }

                        lastMove = input;
                        turn = !turn;
                        set_moving = !set_moving;
                        break;
                    }

                    case 3: { //Case 3: Enemy king in mate after move, game ends
                        clear_output();
                        b.printBoard();
                        if (!lastMove.empty())
                            cout << "\nLast Move: " << lastMove;

                        logger.log(p1, "Moving \"" + input + "\"");

                        cout << "\n\nYou putted " << bot.get_name() << "'s king in mate!\nGame Over!\n";
                        isGameFinished = true;
                        logger.log(p1, "Enemy king in Mate!");
                        break;
                    }
                }
            } 
            else {   //move not executed, handling various cases
                switch (moveOutput.second) {

                    case 1 : {  //move is not valid and the cycle repeats
                        clear_output();
                        cout << "\n\nInvalid move, try again!\n";
                        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                        break;
                    }

                    case 2 : {  //move is not valid, king still in danger after possible move, the cycle repeats
                        clear_output();
                        cout << "\n\nYour king is still in check! Try another move";
                        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
                        break;
                    }

                    case 3 : {  //there is a threefold draw condition, draw requested and waiting for input
                        logger.log(console, "\nThreefold draw has been requested");
                        cout << "\n\nDraw for threefold repetition rule, accept it? [Y/N] : ";
                        do {
                            getline(cin, input);

                            if (tolower(input.front()) == 'y' || tolower(input.front()) == 'n')
                                break;
                        } while (true);

                        if(tolower(input.front()) == 'y') { //the draw is accepted and the game ends
                            logger.log(p1, "Draw accepted.");
                            logger.log(console, "Ending game...");

                            clear_output();
                            b.printBoard();
                            if (!lastMove.empty())
                                cout << "\nLast Move: " << lastMove;

                            cout << "\n\nDraw accepted!\nGame Over!\n";

                            isGameFinished = true;

                            break;
                        }
                        else if(tolower(input.front()) == 'n'){ //the draw is declined and the move is forced to be executed bypassing the check
                            logger.log(p1, "Draw declined.");

                            b.move(start, end, set_moving, false, true);
                            logger.log(p1, "Moving \"" + input + "\"");
                            logger.log(p1, "Moved bypassing draw");

                            clear_output();
                            b.printBoard();
                            if (!lastMove.empty())
                                cout << "\nLast Move: " << lastMove;

                            cout << "\n\nDraw declined! Game goes on";
                            lastMove = input;
                            turn = !turn;
                            set_moving = !set_moving;
                        }
                        break;
                    }

                    case 4: {  //the game is forced to end because of the chessman number
                        clear_output();
                        b.printBoard();
                        if (!lastMove.empty())
                            cout << "\nLast Move: " << lastMove;

                        isGameFinished = true;

                        // Logger part
                        logger.log(p1, "Tie forced due to chessman number, ending game...");
                        // Output Part
                        cout << "\nTie forced for chessman number!\nGame Over!\n";

                        break;
                    }

                    case 5: { //the game is forced to end because of a stalled game
                        clear_output();  
                        b.printBoard();
                        if (!lastMove.empty())
                            cout << "\nLast Move: " << lastMove;

                        isGameFinished = true;

                        // Logger part
                        logger.log(p1, "Tie forced for stalled game, ending game...");
                        // Output Part
                        cout << "\nTie forced for stalled game\nGame Over!\n";

                        break;
                    }

                    case 6: { //the game is forced to end because of the 50 moves rule
                        clear_output();
                        b.printBoard();
                        if (!lastMove.empty())
                            cout << "\nLast Move: " << lastMove;

                        isGameFinished = true;

                        logger.log(p1, "Tie forced for 50 moves rule, ending game...");

                        cout << "\nTie forced for 50 move rule\nGame Over!\n";

                        break;
                    }

                    case 7: {
                        clear_output();
                        cout <<  "\nPlayer " << p1 << " requested a draw, asking to " << bot.get_name() << "\n";
                        logger.log(p1, "requested draw");
                        acceptedDraw = bot.handledraw();
                        if(acceptedDraw == requestDraw){
                            cout << "\n" << bot.get_name() << " accepted the draw request, game is ended\n";
                            logger.log(bot.get_name(), "accepted draw");
                            isGameFinished = true;
                            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                        }
                        else{
                            cout << "\n" << bot.get_name() << " declined the draw, the game continues\n";
                            logger.log(bot.get_name(), "declined draw");
                            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                            requestDraw = false;
                        }
                        break;
                    }
                }
            }
        }
        else{ //BOT TURN 
            requestDraw = bot.requestDraw();
            if(!requestDraw){
                botMove =  bot.generateRandomMove();

                start = botMove.first;
                end = botMove.second;

                moveOutput = b.move(start, end, set_moving, false, false);
            } 
            else moveOutput = make_pair(false, 7);

            if (moveOutput.first) {
                switch (moveOutput.second) {

                    case 1 : { // Case 1: move executed, no other actions required
                        logger.log(bot.get_name(), "Moving \"" + conv_readable(botMove) + "\"");
                        lastMove = conv_readable(botMove);

                        turn = !turn;
                        set_moving = !set_moving;
                        break;
                    }   

                    case 2 : { //Case 2: move executed, handling the promotion, same logic applied to the player
                        logger.log(bot.get_name(), "Moving \"" + conv_readable(botMove) + "\"");
                        lastMove = conv_readable(botMove);

                        pair<bool, bool> promotionRes;
                        string pedinaDaPromuovere = bot.handlePromotion();
                        promotionRes = b.promotion(pedinaDaPromuovere.front(), turn); //no check is needed because of the bot implementation

                        logger.log(bot.get_name(), "Promoting a pawn to \"" + pedinaDaPromuovere + "\"");

                        if (!promotionRes.second) {
                            isGameFinished = true;

                            clear_output();
                            b.printBoard();
                            if (!lastMove.empty())
                                cout << "\nLast move: " << lastMove << "\n";

                            cout << "\n"<<bot.get_name()<<" promotion putted your king in mate!\nGame Over!\n";
                            // Logger part
                            logger.log(bot.get_name(), "Enemy king in Mate");
                            logger.log(bot.get_name(), "Ending game...");
                            break;
                        }

                        turn = !turn;
                        set_moving = !set_moving;
                    }

                    case 3: { //Case 3: Enemy king in mate after move, game ends
                        clear_output();
                        b.printBoard();
                        if (!lastMove.empty())
                            cout << "\nLast move: " << lastMove << "\n";

                        isGameFinished = true;

                        // Logger part
                        logger.log(bot.get_name(), "Enemy king in Mate");
                        logger.log(bot.get_name(), "Ending game...");
                        // Output Part
                        cout << "\n"<<bot.get_name()<<" put your king in mate!\nGame Over!\n";
                        break;
                    }
                }
            }
            else { //handling failed move, same logic has been applied to the player
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

                            cout << "A Threefold Draw was asked to " << bot.get_name() << " and was accepted!\nGame Over!\n";

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
                        cout << "\nTie forced for stalled game\nGame Over!\n";
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

                    case 7: {
                        clear_output();
                        cout <<  "\nPlayer " << bot.get_name() << " requested a draw, do you want to accept? [Y/N]\n";
                        logger.log(bot.get_name(), "requested draw");
                        do {
                            getline(cin, input);

                            if (tolower(input.front()) == 'y' || tolower(input.front()) == 'n')
                                break;
                        } while (true);

                        if(tolower(input.front()) == 'y') { //the draw is accepted and the game ends
                            logger.log(p1, "Draw accepted.");
                            logger.log(console, "Ending game...");

                            clear_output();
                            b.printBoard();
                            if (!lastMove.empty())
                                cout << "\nLast Move: " << lastMove;

                            cout << "\n\nDraw accepted!\nGame Over!\n";

                            isGameFinished = true;

                            break;
                        }
                        else{ //the draw is declined and the move is forced to be executed bypassing the check
                            logger.log(p1, "Draw declined.");
                            requestDraw = false;
                        }
                        break;
                    }   
                }
            }
        }
    }

    logger.log(console, "Game ended");
    logger.log(console, "Ending log session");
}

void computersGame(int max) { //handles bot vs bot  games
    board b;
    bool turn = false;
    bool requestDraw = false;
    bool acceptedDraw = false;
    pair<bool, int> moveOutput;
    pair<coords, coords> botMove;

    int maxMoves = max;
    int currMoves = 0;
    logger logger;
    logger.log(console, "Welcome");
    logger.log(console, "Starting new log session");

    //initializing both bots and logs
    string bot1name = randomName();
    bot bot1(bot1name, false, b);
    string bot2name = randomName();
    while(bot1name == bot2name){
        bot2name = randomName();
    }
    bot bot2(bot2name, true, b);

    logger.log(console, "Initializing player 1 \"" + bot1.get_name() + "\"");
    logger.log(console, "Initializing player 2 \"" + bot2.get_name() + "\"");

    bool isGameEnded = false;

    string lastMove{};
    while (!isGameEnded) {
        //handles the max number of moves to be executed and counts the nunber of moves
        if(currMoves == maxMoves){
            cout << "\nThe setted max number of moves has been reached, the game is forcefully terminated!\n";
            logger.log(console, "Max moves reached");
            isGameEnded = true;
            break;
        }
        currMoves++;
        // Creating move
        bot& moving_bot = (bot1.get_set() == turn) ? bot1 : bot2;
        bot& enemy_bot = (bot1.get_set() == turn) ? bot2 : bot1;

        requestDraw = moving_bot.requestDraw();

        if(!requestDraw){
            botMove = moving_bot.generateRandomMove();

            moveOutput = b.move(botMove.first, botMove.second, turn, false, false);
        }
        else moveOutput = make_pair(false, 7);
    
        if (moveOutput.first) { //same logic applied to the player, please refer to the playergame
            switch (moveOutput.second) {

                case 1: {
                    clear_output();
                    cout << "\n --- "<<moving_bot.get_name()<<"'s turn ---\n";
                    b.printBoard();
                    if (!lastMove.empty())
                        cout << "\nLast move: " << lastMove << "\n";

                    turn = !turn;
                    lastMove = conv_readable(botMove);

                    logger.log(moving_bot.get_name(), "Moving \"" + lastMove + "\"");

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

                    cout << "\n\n Promoting to " << promotionPiece << " a pawn from " << moving_bot.get_name() << "'s set";

                    logger.log(moving_bot.get_name(), "Moving \"" + conv_readable(botMove) + "\"");
                    logger.log(moving_bot.get_name(), "Promoting a pawn to \"" + promotionPiece + "\"");

                    if (!promotionOutput.second) {
                        cout << "\n Promotion putted " << enemy_bot.get_name() << "'s king in mate!\nGame Over!\n";
                        logger.log(moving_bot.get_name(), "Enemy king in Mate");
                        logger.log(console, "Ending game...");
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

                    cout << "\n\n" << moving_bot.get_name() <<" putted " << enemy_bot.get_name() << "'s king in mate!\nGame Over!";

                    logger.log(moving_bot.get_name(), "Moving \"" + conv_readable(botMove) + "\"");
                    logger.log(moving_bot.get_name(), "Enemy king in Mate");
                    logger.log(console, "Ending game...");

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

                    logger.log(console, "Threefold draw has been requested");

                    if(bot2.handledraw()) {
                        logger.log(moving_bot.get_name(), "Draw accepted.");
                        logger.log(console, "Ending game...");

                        cout << moving_bot.get_name()<<"accepted Threefold draw!\n";

                        isGameEnded = true;
                    }
                    else {
                        logger.log(moving_bot.get_name(), "Draw declined.");
                        b.move(botMove.first, botMove.second, turn, false, true);

                        clear_output();
                        cout << "\n --- "<<moving_bot.get_name()<<"'s turn ---\n";
                        b.printBoard();

                        if (!lastMove.empty())
                            cout << "\nLast Move: " << lastMove;

                        lastMove = conv_readable(botMove);

                        logger.log(moving_bot.get_name(), "Moving \"" + lastMove + "\"");
                        logger.log(moving_bot.get_name(), "Moved bypassing draw");

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
                    logger.log(moving_bot.get_name(), "Tie forced due to chessman number, ending game...");

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

                    logger.log(moving_bot.get_name(), "Tie forced for stalled game, ending game...");

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

                    logger.log(moving_bot.get_name(), "Tie forced for 50 moves rule, ending game...");

                    cout << "\nTie forced for 50 move rule\nGame Over!\n";

                    break;
                }

                case 7: {
                        clear_output();
                        cout <<  "\nPlayer " << moving_bot.get_name() << " requested a draw, asking to " << enemy_bot.get_name() << "\n";
                        logger.log(moving_bot.get_name(), "requested draw");
                        acceptedDraw = enemy_bot.handledraw();
                        if(acceptedDraw){
                            cout << "\n" << enemy_bot.get_name() << " accepted the draw request, game is ended\n";
                            logger.log(enemy_bot.get_name(), "accepted draw");
                            isGameEnded = true;
                            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                        }
                        else{
                            cout << "\n" << enemy_bot.get_name() << " declined the draw, the game continues\n";
                            logger.log(enemy_bot.get_name(), "declined draw");
                            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                            requestDraw = false;
                        }
                        break;
                    }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
    logger.log(console, "Game ended");
    logger.log(console, "Ending log session");
}

int main(int argc, char *argv[]) {

    if (argc not_eq 2) {
        cout << "Please enter a valid argument!";
        return 1;
    }

    srand(time(NULL));
    string enter;
    int maxMoves;
    cout << "  ______  __    __   _______      _______.     _______. \n" <<
         " /      ||  |  |  | |   ____|    /       |    /       | \n" <<
         "|  ,----'|  |__|  | |  |__      |   (----`   |   (----` \n" <<
         "|  |     |   __   | |   __|      \\   \\        \\   \\ \n" <<
         "|  `----.|  |  |  | |  |____ .----)   |   .----)   |    \n" <<
         " \\______||__|  |__| |_______||_______/    |_______/    \n" <<
         "           Starting the game, please wait....           \n\n";



    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    //handles the command line argument and calls right function
    enter = argv[1];
    if(enter == "cc") {
        cout << "Choose the max number of moves to execute  before terminating the game: ";
        cin >> maxMoves;
        computersGame(maxMoves);
    }
    else if(enter == "pc") {   
        cout << "\nUsage:\nInput FF to ask for a draw\nInput moves in A1 B2 format. Input is case insentive\nGLHF!";
        playerGame();
    }
    else cout << "Invalid selection for [" << enter << "], please provide a valid argument.\n";

    return 0;
}