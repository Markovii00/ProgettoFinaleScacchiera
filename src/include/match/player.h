#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <list>


class player {
protected:
    std::string& name;
    std::list<std::string> moves;
    bool maiusc;

    const std::string player_1_set{"ptcadr"};
    const std::string player_2_set{"PTCADR"};

public:
    //costruttore, prende in ingresso il nome del giocatore come riferimento e se ha il set in maiuscolo o minuscolo
    player(std::string& name, bool maiusc_set);
    //player(std::string& name,bool maiusc_set, board& b);

    //distruttore
    virtual ~player() {
        moves.clear();
        name.clear();
    };

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

#endif