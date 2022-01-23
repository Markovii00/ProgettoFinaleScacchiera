/**
* @author Tommaso Gabrieli - 2007967 (tommaso.gabrieli.1@studenti.unipd.it)
*/


#include "replay.h"
#include "include/board.h"

#include <iostream>
#include <thread>
#include <list>
#include <logtoolkit.cpp>

using namespace std;

string get_player(fstream& file, int player_num) {

    regex contains_id("Initializing player " + to_string(player_num));
    regex rex("\"(.*?)\"");

    string line;
    smatch match_found;
    while (getline(file, line)) {
        if (regex_search(line, contains_id)) {
            regex_search(line,match_found, rex);
            file.clear();
            file.seekg(0);
            return match_found[1];
        }
    }
    file.clear();
    file.seekg(0);
    return "";
}

deque<pair<string, string>> get_moves(fstream& file) {

    regex contains_id("Moving ");
    regex rex("\"(.*?)\"");
    regex name("- (.*?) -");

    deque<pair<string, string>> moves;

    string line;
    smatch match_found;
    smatch name_player;
    while (getline(file, line)) {
        if (regex_search(line, contains_id)) {
            regex_search(line,match_found, rex);
            regex_search(line,name_player, name);
            moves.emplace_back(name_player[1], match_found[1]);
        }
    }
    file.clear();
    file.seekg(0);
    return moves;
}

deque<string> get_promotion(fstream& file) {

    regex contains_id("Promoting a pawn to ");
    regex rex("\"(.*?)\"");

    deque<string> promotions;

    string line;
    smatch match_found;
    while (getline(file, line)) {
        if (regex_search(line, contains_id)) {
            regex_search(line,match_found, rex);
            promotions.push_back(match_found[1]);
        }
    }
    file.clear();
    file.seekg(0);
    return promotions;
}

deque<bool> get_tie_answers(fstream& file) {

    regex contains_id("threefold");
    regex accepted("accepted");
    regex declined("declined");

    deque<bool> tie_answ;

    string line;
    while (getline(file, line)) {
        if (regex_search(line, contains_id)) {
            if (regex_search(line, accepted))
                tie_answ.push_back(true);
            else if (regex_search(line, declined))
                tie_answ.push_back(false);
        }
    }
    file.clear();
    file.seekg(0);
    return tie_answ;
}

bool get_asked_draw(fstream& file) {

    regex accepted("accepted draw");
    regex contains_asked_draw("requested draw");

    string line;
    while (getline(file, line)) {
        if (regex_search(line, contains_asked_draw)) {
            if (regex_search(line, accepted)) {
                file.clear();
                file.seekg(0);
                return true;
            }
        }
    }
    file.clear();
    file.seekg(0);
    return false;
}

bool game_ended_correctly(fstream& file) {
    regex end_reached("Game ended");

    string line;
    while (getline(file, line)) {
        if (regex_search(line, end_reached)) {
            file.clear();
            file.seekg(0);
            return true;
        }
    }
    file.clear();
    file.seekg(0);
    return false;
}

bool is_valid_log_file(fstream& file) {
    regex contains_log("\\[[0-9]{2}:[0-9]{2}:[0-9]{2}\\] - (.*?) - ");

    string line;
    while (getline(file, line)) {
        if (regex_search(line, contains_log)) {
            file.clear();
            file.seekg(0);
            return true;
        }
    }
    file.clear();
    file.seekg(0);
    return false;
}


void clear_term() {
    system("clear");
}

void video_stampa(fstream& log_file) {
    deque<pair<string, string>> moves = get_moves(log_file);
    deque<string> promotions = get_promotion(log_file);
    deque<bool> tie_answ = get_tie_answers(log_file);

    board b;

    bool end_of_replay = false;
    bool turn = false;
    while (!end_of_replay) {

        if (moves.empty()) {
            if (get_asked_draw(log_file)) {
                cout << "\nA draw was accepted, dropping replay session...\n";
            }else if (!is_valid_log_file(log_file)) {
                cout << "\nNo more moves found, dropping replay session...\n";
            }
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

                    clear_term();
                    cout << "--- " << player << "--- \n";
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
                        cout << "--- " << player << "--- \n";
                        b.printBoard();
                        cout << "\nMove executed: " << move << " with promotion in " << promotions.front() <<"\n";
                        promotions.pop_front();
                        if (!promotion_res.second) {
                            cout << "\nPromotion put in mate opponent king, the game ends here\n";
                            end_of_replay = true;
                        }
                    } else {
                        cout << "\nPromotion gone wrong, stopped replay instance\n";
                        return;
                    }

                    turn = !turn;
                    break;
                }

                case 3: {
                    moves.pop_front();

                    clear_term();
                    cout << "--- " << player << "--- \n";
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
                    return;
                }

                case 2 : {
                    clear_term();
                    cout << "Something went wrong during replay, aborting...";
                    return;
                }

                case 3 : {
                    if (tie_answ.front()) {
                        clear_term();
                        cout << "--- " << player << "--- \n";
                        b.printBoard();

                        cout << "\n\nAttempted Move:" << move;
                        cout << "\nThreefold tie accepted, game ends here";
                        end_of_replay = true;

                    } else {
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
        this_thread::sleep_for(chrono::milliseconds(2000));
    }

}
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
            if (get_asked_draw(log_file)) {
                cout << "\nA draw was accepted, dropping replay session...\n";
                file_output << "\nA draw was accepted, dropping replay session...\n";
            }else if (!is_valid_log_file(log_file)) {
                cout << "\nNo more moves found, dropping replay session...\n";
                file_output << "\nNo more moves found, dropping replay session...\n";
            }
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

    if (!log_file.is_open() || !is_valid_log_file(log_file)) {
        cout << "You must provide a valid log file!\n";
        return 1;
    }

    string player_1_name = get_player(log_file, 1);
    string player_2_name = get_player(log_file, 2);

    bool gameEndedCorrectly = game_ended_correctly(log_file);

    cout << "Game data fetched successfully!\n";
    cout << "Player 1: " << player_1_name << "\n";
    cout << "Player 2: " << player_2_name << "\n";
    if (!gameEndedCorrectly)
        cout << "Game was interrupted in an illegal way, replay could not be trustworthy!\n";
    cout << "Starting replay....\n";

    this_thread::sleep_for(chrono::milliseconds(3500));
    clear_term();

    if (strcmp(argv[1], "v") == 0)
        video_stampa(log_file);
    else if (strcmp(argv[1], "f") == 0)
        file_stampa(log_file);
    else cout << "\nLogger usage: [v] [f] <namefile.log>\n";

    return 0;
}

