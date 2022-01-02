/**
*@file board.hpp
*@author  Francesco Colla - 2007968 (francesco.colla.2@studenti.unipd.it)
*@brief
*
*/

#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "chessman.h"

class board
{
    private:
    bool isWhiteTurn;
    int maxMoves;
    chessman* chessboard[8][8];
    bool isBlack(char request) const;
    //bool isInDanger = false;


    public: 
    board();
    ~board();

    bool findKing();
    //returns true is game is ended
    bool isEnded();
    //returns true if game is against a human opponent, input from main function: 0 = against human, 1 = pc vs pc
    bool isVsUser(int input);
    //returns true if the selected column is empty and contained in the board
    bool acceptableMove(short fromRow, short fromCol, short toRow, short toCol) const;
    //returns true if the king is in a safe position
    bool kingInCheck(bool requestColor);
    bool kingInCheck(short col, short row, bool requestColor);
    //return true if there is a clear way to the destination
    bool clearPath(unsigned short fromCol, unsigned short fromRow, unsigned short toCol, unsigned short toRow);
    bool move(unsigned short fromCol, unsigned short fromRow, unsigned short toCol, unsigned short toRow);
    bool movePawn(unsigned short fromCol, unsigned short fromRow, unsigned short toCol, unsigned short toRow);
    //changes the active player
    void changeTurn();
    //applies the move to the board
    void executeMove(short fromCol, short fromRow, short toCol, short toRow);
    //quits and executes post-game code
    void endGame();
    //throws needed exceptions
    void handleExceptions();

    //returns all the possible cordinates where the king can move
    std::vector<std::pair<short, short>> KingPossibleMoves(short fromRow, short fromCol) const;
    
    //prints current board to terminal (cout)
    void printBoard();
    //prints current board to file 
    void printToLog();
    void promotion(unsigned short pawnCol,unsigned short pawnRow);
    bool castling(unsigned short fromCol, unsigned short fromRow, unsigned short toCol, unsigned short toRow);
    //returns 0 if tile is empty, otherwise returns piece id
    char getName(short row, short col) const;

    //returns board in a string

    std::string to_string(bool fixed_allignment);
    std::string getKing(bool requestColor);

    bool isBlack(char request);

    bool acceptableMove(short fromRow, short fromCol, short toRow, short toCol);
};

#endif