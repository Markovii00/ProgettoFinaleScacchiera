/**
*@file board.hpp
*@author  Francesco Colla - 2007968 (francesco.colla.2@studenti.unipd.it)
*@brief
*
*/

#ifndef BOARD_H_
#define BOARD_H_

#include <vector>
#include <iostream>

#include "src/include/chessman.h"
#include "src/include/bishop.h"
#include "src/include/king.h"
#include "src/include/queen.h"
#include "src/include/knight.h"
#include "src/include/pawn.h"
#include "src/include/rook.h"


class board
{
    private:
    chessman chessboard[8][8];
    bool isWhiteTurn = true;
    sting


    public: 
    board();
    ~board();


    bool isValid();
    bool isFree();
    bool isEnded();
    bool whithinLimits(short col, short row);
    
    
    
    void changeTurn();
    void endGame();
};

#endif