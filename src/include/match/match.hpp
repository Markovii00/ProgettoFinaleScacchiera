//
// Created by Tommaso Gabrieli on 21/12/21.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_MATCH_HPP
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_MATCH_HPP

#include "board.h"

class match {
    player& player_1;
    player& player_2;
    board& b;
    int round = 0;

public:
    match(player& player_1, player& player_2, board& b) : player_1(player_1), player_2(player_2), b(b) {};

    ~match() {
         player_2.~player();
         player_1.~player();
    };

    player& get_player(int num) {
        return (num == 1 ? player_1 : player_2);
    }

    board& get_board() {
        return b;
    }

    int get_round() const {
        return round;
    }

    player& get_player_turn() {
        return (whose_turn() ? player_1 : player_2);
    }

    bool whose_turn() const {
        return (round%2 ==0);
    }

    void next_round() {
        ++round;
    }
};

#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_MATCH_HPP
