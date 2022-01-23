//
// Created by Tomma on 12/18/2021.
//

#include "runnable/replay.h"
#include "include/board.h"

#include <iostream>
#include <thread>
#include <list>
#include <graphic/graphic.h>

using namespace std;

void clear_term() {
    system("clear");
}

int main(int argc, char *argv[]) {

    if (argc not_eq 2) {
        cout << "Logger usage: [-cc] [-pc] <namefile.log>";
        return 1;
    }

    cout << "_______________________________.____       _____ _____.___.\n" <<
            "\\______   \\_   _____/\\______   \\    |     /  _  \\\\__  |   |\n" <<
            " |       _/|    __)_  |     ___/    |    /  /_\\  \\/   |   |\n" <<
            " |    |   \\|        \\ |    |   |    |___/    |    \\____   |\n" <<
            " |____|_  /_______  / |____|   |_______ \\____|__  / ______|\n" <<
            "        \\/        \\/                   \\/       \\/\\/       \n" <<
            "              Replay Starting, please wait....            \n";

    fstream log_file(argv[1]);

    string player_1_name = get_player(log_file, 1);
    string player_2_name = get_player(log_file, 2);

    bool gameEndedCorrectly = game_ended_correctly(log_file);

    board b;

    cout << "Game data fetched successfully!\n";
    cout << "Player 1: " << player_1_name << "\n";
    cout << "Player 2: " << player_2_name << "\n";
    if (!gameEndedCorrectly)
        cout << "Game was interrupted in an illegal way, replay could not be trustworthy!\n";
    cout << "Starting replay....\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(3500));
    clear_term();


    deque<string> moves = get_moves(log_file);
    deque<string> promotions = get_promotion(log_file);
    deque<bool> tie_answ = get_tie_answers(log_file);

    bool end_of_replay = false;
    bool turn = false;
    while (!end_of_replay) {

        string move = moves.front();


        coords start;
        start.first = 8-(move.at(1) - '0');
        start.second = toupper(move.at(0)-65);

        coords end;
        end.first = 8-(move.at(4)-'0');
        end.second = tolower(move.at(3)-65);

        cout << start.first << "  " << start.second << " " << end.first <<" " << end.second;

        pair<bool, int> move_return = b.move(start, end,turn, false, false);

        if (move_return.first) {
            switch (move_return.second) {

                case 1: {
                    moves.pop_front();

                    clear_term();
                    b.printBoard();
                    cout << "\nMove executed: " << move << "\n";
                    turn = !turn;
                    break;
                }

                case 2: {
                    moves.pop_front();

                    pair<bool, bool> promotion_res = b.promotion(promotions.front().front(), turn);

                    if (promotion_res.first) {
                        clear_term();
                        b.printBoard();
                        cout << "\nMove executed: " << move << " with promotion in " << promotions.front() <<"\n";
                        promotions.pop_front();
                        if (!promotion_res.second) {
                            cout << "\nPromotion put in mate opponent king, the game ends here\n";
                            end_of_replay = true;
                        }
                    } else {
                        cout << "\nPromotion gone wrong, stopped replay instance\n";
                        return 1;
                    }

                    turn = !turn;
                    break;
                }

                case 3: {
                    moves.pop_front();

                    clear_term();
                    b.printBoard();
                    cout << "\nMove executed: " << move << " putted enemy in mate, game ends here" <<"\n";
                    end_of_replay = true;
                    break;
                }


            }
        } else {
            switch (move_return.second) {

                case 1 : {
                    clear_term();
                    cout << "Something went wrong during replay, aborting...";
                    return 1;
                }

                case 2 : {
                    clear_term();
                    cout << "Something went wrong during replay, aborting...";
                    return 1;
                }

                case 3 : {
                    if (tie_answ.front()) {
                        clear_term();
                        b.printBoard();

                        cout << "\n\nAttempted Move:" << move;
                        cout << "\nThreefold tie accepted, game ends here";
                        end_of_replay = true;

                    } else {
                        b.move(start, end, turn, false, true);
                        clear_term();
                        b.printBoard();
                        cout << "\n\nExecuted Move:" << move;
                        cout << "\nThreefold tie declined, game goes on";
                        moves.pop_front();
                        tie_answ.pop_front();
                        turn = !turn;
                    }
                    break;
                }

                case 4 : {
                    clear_term();
                    b.printBoard();
                    cout << "\n\nGame ends by a tie for pieces";
                    end_of_replay = true;
                    break;
                }

                case 5 : {
                    clear_term();
                    b.printBoard();
                    cout << "\n\nGame ends by a tie for stalled game";
                    end_of_replay = true;
                    break;
                }

                case 6 : {
                    clear_term();
                    b.printBoard();
                    cout << "\n\nGame ends by a tie for 50 move rule";
                    end_of_replay = true;
                    break;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }

    return 0;
}

