//
// Created by Tommaso Gabrieli on 21/12/21.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_PLAYER_HPP
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_PLAYER_HPP

#include <iostream>
#include "../chessman.h"
#include <list>

const std::string player_1_set{"PTCADR"};
const std::string player_2_set{"ptcadr"};

struct player {
private:
    std::string& name;
    std::list<std::string> moves;
    bool is_main;

public:
    explicit player(std::string& name, bool is_main = false) : name(name), is_main(is_main) {};

    ~player() {
        moves.clear();
        name.clear();
    };

    std::string& get_name() const {
        return name;
    }

    std::list<std::string> get_moves() const {
        return moves;
    }

    bool is_in_set(const char* chess_str_representation) const {
        if(is_main)
            return player_1_set.find(chess_str_representation);
        else
            return player_2_set.find(chess_str_representation);
    }
};


#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_PLAYER_HPP
