//
// Created by Tomma on 12/21/2021.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_REPLAY_H
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_REPLAY_H
#include <iostream>
#include <fstream>
#include <regex>
#include <match/player.hpp>
#include <list>
#include <match/match.hpp>

/*
 * Recupera l'id della sessione, per identificare una partita
 *
 */
std::string get_log_number(std::fstream& file) {

    std::regex contains_id("Log session number");
    std::regex id("[0-9]{6}");

    std::string line;
    std::smatch match_found;
    while (std::getline(file, line)) {
        if (std::regex_search(line, contains_id)) {
            std::regex_search(line,match_found, id);
            file.clear();
            file.seekg(0);
            return match_found[0];
        }
    }
    file.clear();
    file.seekg(0);
    return "";
}

/*
 * recupera dal fstrem il nome del giocatore 1 o 2
 *
 */
std::string get_player(std::fstream& file, int player_num) {

    std::regex contains_id("Initializing player " + std::to_string(player_num));
    std::regex rex("\"(.*?)\"");

    std::string line;
    std::smatch match_found;
    while (std::getline(file, line)) {
        if (std::regex_search(line, contains_id)) {
            std::regex_search(line,match_found, rex);
            file.clear();
            file.seekg(0);
            return match_found[1];
        }
    }
    file.clear();
    file.seekg(0);
    return "";
}

/*
 * dato un fstream, un player e una lista, inserisce all'interno della lista le mosse fatte da un giocatore
 * secondo i log
 *
 */
void get_player_moves(std::fstream& file, player& p, std::list<std::string>& moves) {
    std::string rgx{"- "+ p.get_name() +" - Moving "};
    std::regex player_move_row(rgx);
    std::regex player_move("\"(.*?)\"");

    std::string line;
    std::smatch s;
    while (std::getline(file, line)) {
        if (std::regex_search(line, player_move_row)) {
            std::regex_search(line,s, player_move);
            moves.push_back(s[1]);
        }
    }
    file.clear();
    file.seekg(0);
}


#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_REPLAY_H
