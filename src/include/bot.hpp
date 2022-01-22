#ifndef BOT_HPP
#define BOT_HPP

#include <iostream>
#include "../chessman.h"
#include "../board.h"
#include <list>
#include <vector>
#include <string>


class bot {
private:
    std::string& name;
    std::vector<coords> updatedSet;
    std::vector<coords> allMoves;
    coords start;
    coords end;
    board& b;
    short indexFrom;
    short indexTo;
    bool maiusc;
    short randPromotion;
    std::vector<char>PromotionSet = {'d', 't', 'a', 'c'};

public:
    //costruttore, prende in ingresso il nome del giocatore come riferimento e se ha il set in maiuscolo o minuscolo
    explicit bot(std::string& namevar,bool set, board& bvar) : name(namevar), b(bvar){
        name = namevar;
        maiusc = set;
        b = bvar;
    };

    //distruttore
    ~bot() {
        name.clear();
        updatedSet.clear();
        allMoves.clear();
    };

    //ritorna il nome del giocatore
    std::string& get_name() const { return name; }

    std::pair<coords, coords> generateRandomMove();

    //cambia il nome al giocatore
    void set_name(std::vector<std::string>names);

    bool handledraw();

    bool requestDraw();

    char handlePromotion();

};

#endif