//
// Created by Tomma on 12/18/2021.
//

#include "include/runnable/replay.h"
#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

int main(int argc, char *argv[]) {

    if (argc not_eq 3) {
        cout << "Logger usage: [-cc] [-pc] <namefile.log>";
        return 1;
    }

    cout << "-.-.-.-.- THE CHESS GAME -.-.-.-.-" << "\n";

    fstream log_file(argv[1]);

    string play_id = get_log_number(log_file);

    string player_1_name = get_player(log_file, 1);
    string player_2_name = get_player(log_file, 2);

    cout << "Dati della partita \n";
    cout << "Numero del file log: " << play_id << "\n";
    cout << "Nome del player 1: " << player_1_name << "\n";
    cout << "Nome del player 2: " << player_2_name << "\n";

}
