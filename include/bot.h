/**
 * @author Francesco Colla - 2007968 (francesco.colla.2@studenti.unipd.it)
 */

#ifndef BOT_H
#define BOT_H

#include <iostream>
#include "chessman.h"
#include "board.h"
#include <vector>
#include <string>
#include <random>

class bot {
private:
    std::string name;
    board& b;
    bool maiusc;

    std::vector<coords> updatedSet;
    std::vector<coords> allMoves;
    coords start;
    coords end;
    short indexFrom;
    short indexTo;
    short randPromotion;
    std::vector<std::string> promotionSet = {"d", "t", "a", "c"};

public:
    /**
     * @param n name of the bot
     * @param set bot is playing either with black set (false) or white set(true)
     * @param bVar chessboard where to bot is playing
     */
    explicit bot(std::string &n, bool set, board &bVar); 
    ~bot();

    /**
     * @brief call the method in board called getSetPossibleMoves, then choose one possible move from it
     * @return randomly generated move
     */
    std::pair<coords, coords> generateRandomMove(void);

    /**
     * @brief 5% chance that the bot will accept a draw
     * @return true draw accepted, false instead
     */
    bool handledraw();

    /**
     * @brief 1% chance that the bot will propose a draw
     * @return true draw proposed, false instead
     */
    bool requestDraw();

    /**
     * @return string of a randomly selected piece in which promote the pawn
     */
    std::string handlePromotion();


    std::string get_name() const;
    bool get_set() const;
};

#endif