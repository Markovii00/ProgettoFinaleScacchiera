/**
*@file board.hpp
*@author  Francesco Colla - 2007968 (francesco.colla.2@studenti.unipd.it)
*@brief
*
*/

#ifndef BOARD_H_
#define BOARD_H_

#include <iostream>

#include "include/chessman.h"
#include "include/bishop.h"
#include "include/king.h"
#include "include/queen.h"
#include "include/knight.h"
#include "include/pawn.h"
#include "include/rook.h"


class board
{
    private:
    bool isWhiteTurn;
    int maxMoves;
    chessman* chessboard[8][8];
    //bool isInDanger = false;


    public: 
    board();
    ~board();


    //returns true is game is ended
    bool isEnded();
    //returns true if game is against a human opponent, input from main function: 0 = against human, 1 = pc vs pc
    bool isVsUser(int input);
    //returns true if the selected column is empty and contained in the board
    bool isTargetValid(short col, short row);
    //returns true if the king is in a safe position
    bool kingNotSafe(char pieceIdentity);
    
    //changes the active player
    void changeTurn();
    //applies the move to the board
    void executeMove(short fromCol, short fromRow, short toCol, short toRow);
    //eliminates a piece in the given square
    void eliminatePiece(short col, short row);
    //quits and executes post-game code
    void endGame();
    //throws needed exceptions
    void handleExceptions();

    //prints current board to terminal (cout)
    void printBoard();
    //prints current board to file 
    void printToLog();
};

#endif