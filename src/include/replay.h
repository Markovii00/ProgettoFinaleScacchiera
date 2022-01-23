/**
* @author Tommaso Gabrieli - 2007967 (tommaso.gabrieli.1@studenti.unipd.it)
*/


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

std::deque<std::pair<std::string, std::string>> get_moves(std::fstream& file) {

    std::regex contains_id("Moving ");
    std::regex rex("\"(.*?)\"");
    std::regex name("- (.*?) -");

    std::deque<std::pair<std::string, std::string>> moves;

    std::string line;
    std::smatch match_found;
    std::smatch name_player;
    while (std::getline(file, line)) {
        if (std::regex_search(line, contains_id)) {
            std::regex_search(line,match_found, rex);
            std::regex_search(line,name_player, name);
            moves.emplace_back(name_player[1], match_found[1]);
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

bool get_asked_draw(std::fstream& file) {

    std::regex accepted("accepted draw");
    std::regex contains_asked_draw("requested draw");

    std::string line;
    while (std::getline(file, line)) {
        if (std::regex_search(line, contains_asked_draw)) {
            if (std::regex_search(line, accepted)) {
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

bool is_valid_log_file(std::fstream& file) {
    std::regex contains_log("\\[[0-9]{2}:[0-9]{2}:[0-9]{2}\\] - (.*?) - ");

    std::string line;
    while (std::getline(file, line)) {
        if (std::regex_search(line, contains_log)) {
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
