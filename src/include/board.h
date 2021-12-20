/**
*@file board.hpp
*@author  Francesco Colla - 2007968 (francesco.colla.2@studenti.unipd.it)
*@brief
*
*/

#ifndef BOARD_H_
#define BOARD_H_

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
    chessman* chessboard[8][8];
    bool isWhiteTurn;
    int maxMoves;
    //bool isInDanger = false;


    public: 
    board();
    ~board();

    //returns true is game is ended
    bool isEnded();
    //returns true if game is against a human opponent, input from main function: 0 = against human, 1 = pc vs pc
    bool isVsUser(int input);
    //returns true if the selected column is empty and contained in the board
    bool destIsValid(short col, short row);
    //returns true if the king is in a safe position
    bool safeKing(char pieceColor);
    //returns true if the source position is valid
    bool sourceIsValid(short col, short row);
    
    //changes the active player
    void changeTurn();
    //applies the move to the board
    void executeMove(char fromCol, char fromRow, char toCol, char toRow);
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