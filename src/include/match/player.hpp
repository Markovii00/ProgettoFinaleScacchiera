//
// Created by Tommaso Gabrieli on 21/12/21.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_PLAYER_HPP
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_PLAYER_HPP

#include <iostream>
#include "../chessman.h"
#include <list>

const std::string player_1_set{"ptcadr"};
const std::string player_2_set{"PTCADR"};

class player {
private:
    std::string& name;
    std::list<std::string> moves;
    bool maiusc;

public:
    explicit player(std::string& name, bool maiusc_set = false) : name(name), maiusc(maiusc_set) {};

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

    //duplicato in board.h, ricordarsi di cancellare uno dei due
    bool is_in_set(const char* chess_str_representation) const {
        if (!maiusc)
            return player_1_set.find(chess_str_representation);
        else
            return player_2_set.find(chess_str_representation);
    }

    void add_move(std::string* move) {
        moves.push_back(*move);
    }
};


#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_PLAYER_HPP
