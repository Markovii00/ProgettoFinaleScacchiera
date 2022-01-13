//
// Created by Tommaso Gabrieli on 21/12/21.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_MATCH_HPP
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_MATCH_HPP

#include "../board.h"

class match {
    player& player_1;
    player& player_2;
    board& b;
    int starter;
    int round = 0;

public:

    //inizializza un match, prende in ingresso il player 1, player 2 e una board
    match(player& player_1, player& player_2, board& b, int starter) : player_1(player_1), player_2(player_2), b(b), starter(starter) {
        if(starter == 1){
            round == 0;
        }
        else{
            round == 1;
        }
    };

    ~match() {
         player_2.~player();
         player_1.~player();
    };

    /*
     * ritorna, dato il numero 1 o 2, il player della partita
     */
    player& get_player(int num) {
        return (num == 1 ? player_1 : player_2);
    }


    /*
     * ritorna la board della partita
     */
    board& get_board() {
        return b;
    }

    /*
     * ritorna il round
     */
    int get_round() const {
        return round;
    }

    /*
     * ritorna il giocatore di cui è il truno
     */
    player& get_player_turn() {
        return (whose_turn() ? player_1 : player_2);
    }

    /*
     * ritorna il giocatore di cui è il turno sotto la forma di un booleano:
     *      - true: player 1
     *      - false: player 2
     */
    bool whose_turn() const {
        return (round%2 ==0);
    }

    /*
     * incrementa il round
     */
    void next_round() {
        ++round;
    }
};

#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_MATCH_HPP
