//
// Created by Tommaso Gabrieli on 21/12/21.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_MATCH_HPP
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_MATCH_HPP

#include "board.h"

struct match {
    player& player_1;
    player& player_2;
    board& board;
    int round = 1;
    bool who = false;

public:
    match(player& player_1, player& player_2, class board& board) : player_1(player_1), player_2(player_2), board(board) {};

    ~match() {
         player_2.~player();
         player_1.~player();
    };

    player& get_player(int num) {
        return (num == 1 ? player_1 : player_2);
    }
};

#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_MATCH_HPP
