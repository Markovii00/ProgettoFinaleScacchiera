/**
*@file board.hpp
*@author  Francesco Colla - 2007968 (francesco.colla.2@studenti.unipd.it)
*@brief
*
*/

#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <iostream>

#include "src/include/chessman.hpp"
#include "src/include/bishop.hpp"
#include "src/include/king.hpp"
#include "src/include/queen.hpp"
#include "src/include/knight.hpp"
#include "src/include/pawn.hpp"
#include "src/include/rook.hpp"


class board
{
    private:
    chessman chessboard[8][8];
    bool isWhiteTurn = true;


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