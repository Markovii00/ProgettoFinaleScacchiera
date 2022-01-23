//
// Created by Tomma on 12/21/2021.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_REPLAY_H
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_REPLAY_H
#include <iostream>
#include <fstream>
#include <regex>
#include <list>

/*
 * Recupera il nome del player 1 e 2
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

std::deque<std::string> get_moves(std::fstream& file) {

    std::regex contains_id("Moving ");
    std::regex rex("\"(.*?)\"");

    std::deque<std::string> moves;

    std::string line;
    std::smatch match_found;
    while (std::getline(file, line)) {
        if (std::regex_search(line, contains_id)) {
            std::regex_search(line,match_found, rex);
            moves.push_back(match_found[1]);
        }
    }
    file.clear();
    file.seekg(0);
    return moves;
}

std::deque<std::string> get_promotion(std::fstream& file) {

    std::regex contains_id("Promoting a pawn to ");
    std::regex rex("\"(.*?)\"");

    std::deque<std::string> promotions;

    std::string line;
    std::smatch match_found;
    while (std::getline(file, line)) {
        if (std::regex_search(line, contains_id)) {
            std::regex_search(line,match_found, rex);
            promotions.push_back(match_found[1]);
        }
    }
    file.clear();
    file.seekg(0);
    return promotions;
}

std::deque<bool> get_tie_answers(std::fstream& file) {

    std::regex contains_id("threefold");
    std::regex accepted("accepted");
    std::regex declined("declined");

    std::deque<bool> tie_answ;

    std::string line;
    while (std::getline(file, line)) {
        if (std::regex_search(line, contains_id)) {
            if (std::regex_search(line, accepted))
                tie_answ.push_back(true);
            else if (std::regex_search(line, declined))
                tie_answ.push_back(false);
        }
    }
    file.clear();
    file.seekg(0);
    return tie_answ;
}

bool game_ended_correctly(std::fstream& file) {
    std::regex end_reached("Game ended");

    std::string line;
    while (std::getline(file, line)) {
        if (std::regex_search(line, end_reached)) {
            file.clear();
            file.seekg(0);
            return true;
        }
    }
    file.clear();
    file.seekg(0);
    return false;
}


#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_REPLAY_H
