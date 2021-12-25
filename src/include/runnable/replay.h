//
// Created by Tomma on 12/21/2021.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_REPLAY_H
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_REPLAY_H
#include <iostream>
#include <fstream>
#include <regex>

std::string get_log_number(std::fstream& file) {

    std::regex contains_id("Log session number");
    std::regex id("[0-9]{6}");

    std::string line;
    std::smatch match_found;
    while (std::getline(file, line)) {
        if (std::regex_search(line, contains_id)) {
            std::regex_search(line,match_found, id);
            return match_found[0];
        }
    }

    //throw NoMatchFound("No match found for \"id\" parameter standard regex: [0-9]{6}");
}

std::string get_player(std::fstream& file, int player_num) {

    std::regex contains_id("Initializing player " + std::to_string(player_num));
    std::regex rex("\"(.*?)\"");

    std::string line;
    std::smatch match_found;
    while (std::getline(file, line)) {
        if (std::regex_search(line, contains_id)) {
            std::regex_search(line,match_found, rex);
            return match_found[1];
        }
    }

    //throw NoMatchFound("No match found for \"player "+ std::to_string(player_num) +"\" parameter standard regex: \"(.*?)\".");
}


#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_REPLAY_H
