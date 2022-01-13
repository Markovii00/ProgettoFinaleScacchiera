//
// Created by Tommaso Gabrieli on 21/12/21.
//

#ifndef PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_USER_HPP
#define PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_USER_HPP

#include <iostream>
#include "player.h"
#include "../chessman.h"
#include <list>

const std::string player_1_set{"ptcadr"};
const std::string player_2_set{"PTCADR"};

class user : public player{
private:
    std::string& name;
    std::list<std::string> moves;
    bool maiusc;

public:
    //costruttore, prende in ingresso il nome del giocatore come riferimento e se ha il set in maiuscolo o minuscolo
    explicit user(std::string& name, bool maiusc_set = false) : name(name), maiusc(maiusc_set) {};

    //distruttore
    ~user() {
        moves.clear();
        name.clear();
    };

    //ritorna il nome del giocatore
    std::string& get_name() const {
        return name;
    }

    //cambia il nome al giocatore
    void set_name(std::string n) {
        name.assign(n.begin(), n.end());
    }

    //ritorna la lista delle mosse
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
    // aggiunge una mossa nella lista, nel fomrato INIZIALE-FINALE eg: C1-B3
    void add_move(std::string* move) {
        moves.push_back(*move);
    }

    // Data una lista, copia le mosse all'interno
    void import_moves(std::list<std::string>& new_moves) {
        moves.assign(new_moves.begin(), new_moves.end());
    }
};


#endif //PROGETTO_FINALE_SCACCHIERA_ELETTRONICA_PLAYER_HPP
