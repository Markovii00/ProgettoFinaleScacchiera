/**
* @author Tommaso Gabrieli - 2007967 (tommaso.gabrieli.1@studenti.unipd.it)
*/


#include "../include/board.h"

#include <iostream>
#include <thread>
#include <list>
#include "../include/log/logtoolkit.h"
#include "../include/replay.h"
#include <regex>

using namespace std;

//Clears console for a fancy and clean cout
void clear_term() {
    system("clear");
}

// Handle v argument option
void video_stampa(fstream& log_file) {

    //gets in a deque moves, all promotions and tie asks
    deque<pair<string, string>> moves = get_moves(log_file);
    deque<string> promotions = get_promotion(log_file);
    deque<bool> tie_answ = get_tie_answers(log_file);

    //initialize the board
    board b;

    // Breaks while loop at the end of the replay
    bool end_of_replay = false;

    //starts always white
    bool turn = false;
    while (!end_of_replay) {

        //if moves deque is empty, replay is finished
        if (moves.empty()) {
            cout << "\nNo more moves found, dropping replay session...\n";
            break;
        }

        //Deque is made up of pairs, in witch there´s saved the player who has done the move and the move in a readable format
        string move = moves.front().second;
        string player = moves.front().first;

        //Convert coordinates from a human format to array matrix format
        coords start;
        start.first = 8-(move.at(1) - '0');
        start.second = toupper(move.at(0))-65;

        coords end;
        end.first = 8-(move.at(4)-'0');
        end.second = toupper(move.at(3))-65;

        // Execute move and get the result
        pair<bool, int> move_return = b.move(start, end,turn, false, false);

        //If move has executed
        if (move_return.first) {
            // Switches the result code
            switch (move_return.second) {

                //In thgis case, move has executed and nothing happened
                case 1: {
                    //Removes move from front
                    moves.pop_front();

                    // Fancy clean term
                    clear_term();
                    //Prints who has executed the move, the board and the move executed
                    cout << "-------- " << player << " -------- \n\n";
                    b.printBoard();
                    cout << "\nMove executed: " << move << "\n";
                    turn = !turn;
                    break;
                }

                // In this other case, a promotion is asked
                case 2: {
                    //Removes move from front
                    moves.pop_front();

                    //get from the deque the promotion asked about
                    pair<bool, bool> promotion_res = b.promotion(promotions.front().front(), turn);

                    // If promotion was done successfully
                    if (promotion_res.first) {

                        //Cout the board, move, promotion and player who has done the promotion
                        clear_term();
                        cout << "-------- " << player << " -------- \n\n";
                        b.printBoard();
                        cout << "\nMove executed: " << move << " with promotion in " << promotions.front() <<"\n";

                        // Removes promotion from deque
                        promotions.pop_front();

                        // If the second element of the pair returned by the board::promote is false, enemy king is in mate, the replay stops;
                        if (!promotion_res.second) {
                            cout << "\nPromotion put in mate opponent king, the game ends here\n";
                            end_of_replay = true;
                        }
                    } else {
                        // If the first element returned from the board::promotion is false, then there is a problem on your log file;

                        cout << "\nPromotion gone wrong, stopped replay instance\n";
                        return;
                    }

                    turn = !turn;
                    break;
                }

                // In this case, after a move the enemy king is in checkmate, the game ends
                case 3: {
                    moves.pop_front();

                    clear_term();
                    cout << "-------- " << player << " -------- \n\n";
                    b.printBoard();
                    cout << "\nMove executed: " << move << " putted enemy in mate, game ends here" <<"\n";
                    end_of_replay = true;
                    break;
                }
            }
        } else {  //Move not executed, there is an error?
            switch (move_return.second) {

                // There is a critical error on your log file, the replay can´t resume
                case 1 : {
                    clear_term();
                    cout << "Something went wrong during replay, aborting...";
                    return;
                }

                case 2 : {
                    clear_term();
                    cout << "Something went wrong during replay, aborting...";
                    return;
                }

                // A threefold draw has asked:
                case 3 : {
                    //if true, the draw has accepted, replay ends
                    if (tie_answ.front()) {

                        clear_term();
                        cout << "--- " << player << "--- \n";
                        b.printBoard();

                        cout << "\n\nAttempted Move:" << move;
                        cout << "\nThreefold tie accepted, game ends here";
                        end_of_replay = true;

                    } else {  //if false, the draw has declines, replay resume forcing the move
                        b.move(start, end, turn, false, true);
                        clear_term();
                        cout << "--- " << player << "--- \n";
                        b.printBoard();
                        cout << "\n\nExecuted Move:" << move;
                        cout << "\nThreefold tie declined, game goes on";
                        moves.pop_front();
                        tie_answ.pop_front();
                        turn = !turn;
                    }
                    break;
                }

                // Theese last cases returns, as wrtitten in the cout, last three case of tie: draft for pieces, moverule50 and game stalled
                case 4 : {
                    clear_term();
                    cout << "--- " << player << "--- \n";
                    b.printBoard();
                    cout << "\n\nGame ends by a tie for pieces";
                    end_of_replay = true;
                    break;
                }

                case 5 : {
                    clear_term();
                    cout << "--- " << player << "--- \n";
                    b.printBoard();
                    cout << "\n\nGame ends by a tie for stalled game";
                    end_of_replay = true;
                    break;
                }

                case 6 : {
                    clear_term();
                    cout << "--- " << player << "--- \n";
                    b.printBoard();
                    cout << "\n\nGame ends by a tie for 50 move rule";
                    end_of_replay = true;
                    break;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

}
// This is basically the same thing as video cout, the only difference is that there is a
// limited number of cout and all the boards are printed inside a file witch the name is prompted at the end of the task
void file_stampa(fstream& log_file) {
    deque<pair<string, string>> moves = get_moves(log_file);
    deque<string> promotions = get_promotion(log_file);
    deque<bool> tie_answ = get_tie_answers(log_file);

    ofstream file_output;
    string file_name = create_replay_output_name();
    file_output.open(file_name);

    board b;

    bool end_of_replay = false;
    bool turn = false;
    while (!end_of_replay) {

        if (moves.empty()) {
                cout << "\nNo more moves found, dropping replay session...\n";
                file_output << "\nNo more moves found, dropping replay session...\n";
            break;
        }

        string move = moves.front().second;
        string player = moves.front().first;


        coords start;
        start.first = 8-(move.at(1) - '0');
        start.second = toupper(move.at(0))-65;

        coords end;
        end.first = 8-(move.at(4)-'0');
        end.second = toupper(move.at(3))-65;

        pair<bool, int> move_return = b.move(start, end,turn, false, false);

        if (move_return.first) {
            switch (move_return.second) {

                case 1: {
                    moves.pop_front();

                    file_output << "--- " << player << "--- \n";
                    file_output << b.to_string();
                    file_output << "\nMove executed: " << move << "\n";
                    file_output << "\n\n\n\n\n";

                    turn = !turn;
                    break;
                }

                case 2: {
                    moves.pop_front();

                    pair<bool, bool> promotion_res = b.promotion(promotions.front().front(), turn);

                    if (promotion_res.first) {
                        file_output << "--- " << player << "--- \n";
                        file_output << b.to_string();
                        file_output << "\nMove executed: " << move << " with promotion in " << promotions.front() <<"\n";

                        promotions.pop_front();
                        if (!promotion_res.second) {
                            file_output << "\nPromotion put in mate opponent king, the game ends here\n";
                            end_of_replay = true;
                        }
                    } else {
                        file_output << "\nPromotion gone wrong, stopped replay instance\n";
                        return;
                    }
                    file_output << "\n\n\n\n\n";
                    turn = !turn;
                    break;
                }

                case 3: {
                    moves.pop_front();

                    file_output << "--- " << player << "--- \n";
                    file_output << b.to_string();
                    file_output << "\nMove executed: " << move << " putted enemy in mate, game ends here" <<"\n";
                    end_of_replay = true;
                    break;
                }
            }
        } else {
            switch (move_return.second) {

                case 1 : {
                    clear_term();
                    cout << "Something went wrong during replay, aborting...";
                    return;
                }

                case 2 : {
                    clear_term();
                    cout << "Something went wrong during replay, aborting...";
                    return;
                }

                case 3 : {
                    if (tie_answ.front()) {
                        cout << "--- " << player << "--- \n";
                        file_output << b.to_string();

                        file_output << "\n\nAttempted Move:" << move;
                        file_output << "\nThreefold tie accepted, game ends here";
                        end_of_replay = true;

                    } else {
                        b.move(start, end, turn, false, true);

                        file_output << "--- " << player << "--- \n";
                        file_output << b.to_string();
                        file_output << "\n\nExecuted Move:" << move;
                        file_output << "\nThreefold tie declined, game goes on";
                        file_output << "\n\n\n\n\n";

                        moves.pop_front();
                        tie_answ.pop_front();
                        turn = !turn;
                    }
                    break;
                }

                case 4 : {
                    file_output << "--- " << player << "--- \n";
                    file_output << b.to_string();
                    file_output << "\n\nGame ends by a tie for pieces";
                    end_of_replay = true;
                    break;
                }

                case 5 : {
                    file_output << "--- " << player << "--- \n";
                    file_output << b.to_string();
                    file_output << "\n\nGame ends by a tie for stalled game";
                    end_of_replay = true;
                    break;
                }

                case 6 : {
                    file_output << "--- " << player << "--- \n";
                    file_output << b.to_string();
                    file_output <<"\n\nGame ends by a tie for 50 move rule";
                    end_of_replay = true;
                    break;
                }
            }
        }
    }

    cout << "\nWrited replay inside \"" << file_name << "\n";

}

int main(int argc, char *argv[]) {

    // If arguments are not at least three, program dies.
    if (argc not_eq 3) {
        cout << "Logger usage: [v] [f] <namefile.log>\n";
        return 1;
    }

    cout << "_______________________________.____       _____ _____.___.\n" <<
            "\\______   \\_   _____/\\______   \\    |     /  _  \\\\__  |   |\n" <<
            " |       _/|    __)_  |     ___/    |    /  /_\\  \\/   |   |\n" <<
            " |    |   \\|        \\ |    |   |    |___/    |    \\____   |\n" <<
            " |____|_  /_______  / |____|   |_______ \\____|__  / ______|\n" <<
            "        \\/        \\/                   \\/       \\/\\/       \n\n" <<
            "              Replay Starting, please wait....            \n\n\n";

    fstream log_file(argv[2]);

    // If the given file is not a valid log file or doesn't exists, program dies.
    if (!log_file.is_open() || !is_valid_log_file(log_file)) {
        cout << "You must provide a valid log file!\n";
        return 1;
    }

    //Get player Names
    string player_1_name = get_player(log_file, 1);
    string player_2_name = get_player(log_file, 2);

    //Checks if the game has ended in a legal way or in a unexpected way (like ctrl+c)
    bool gameEndedCorrectly = game_ended_correctly(log_file);

    cout << "Game data fetched successfully!\n";
    cout << "Player 1: " << player_1_name << "\n";
    cout << "Player 2: " << player_2_name << "\n";
    // Our log system is pretty safe from corruption, but as everything made by humans is not perfect, so a game killed illegally could have corrupted all the logs.
    if (!gameEndedCorrectly)
        cout << "Game was interrupted in an illegal way, replay could not be trustworthy!\n";
    cout << "Starting replay....\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(3500));
    clear_term();

    // Divides your input in a valid input -> prompts you to selected replay display or kills the session if the second argument is illegal
    if (strcmp(argv[1], "v") == 0)
        video_stampa(log_file);
    else if (strcmp(argv[1], "f") == 0)
        file_stampa(log_file);
    else cout << "\nLogger usage: [v] [f] <namefile.log>\n";

    return 0;
}

