//
// Created by Markovii on 23/01/22.
//

#ifndef REPLAY_REPLAY_H
#define REPLAY_REPLAY_H

#include <iostream>
#include <fstream>
#include <regex>
#include <list>

//gets player from a file
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

//gets move from a file stream
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

//Gets all the promotions made during the game
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

//Gets all tie (ONLY THREEFOLD) in witch is required a force in board::move, a tie between two players is simply treated as end of a log, is useful only for a complete debug;
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


#endif //REPLAY_REPLAY_H
