/**
*@file board.hpp
*@author  Francesco Colla - 2007968 (francesco.colla.2@studenti.unipd.it)
*@brief
*
*/

#ifndef BOARD_H
#define BOARD_H

#include "chessman.h"
#include <vector>
#include <utility>
#include <string>

class board
{
    private:
    chessman* chessboard[8][8];
    bool isWhiteTurn;
    int maxMoves;
    bool possibleEnPassant;
    std::pair<short, short> freezeCordinateEnPassant;
    bool isBlack(char request) const;
    bool isPawn(char pieceId) const;
    bool isKing(char pieceId) const;
    bool isQueen(char pieceId) const;
    bool isBishop(char pieceId) const;
    bool isKnight(char pieceId) const;
    bool isRook(char pieceId) const;
    bool isVertical(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol) const;
    bool isHorizontal(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol) const;
    bool isDiagonal(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol) const;

    
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
    bool acceptableMove(short fromRow, short fromCol, short toRow, short toCol, char fromPieceId) const;
    //returns true if the king is in a safe position
    bool kingInCheck(bool requestColor);
    bool kingInCheck(short col, short row, bool requestColor);
    //return true if there is a clear way to the destination
    bool clearPath(unsigned short fromCol, unsigned short fromRow, unsigned short toCol, unsigned short toRow, char fromPieceId);
    bool move(unsigned short fromCol, unsigned short fromRow, unsigned short toCol, unsigned short toRow);
    bool movePawn(unsigned short fromCol, unsigned short fromRow, unsigned short toCol, unsigned short toRow, bool fromPieceColor);
    //changes the active player
    void changeTurn();
    //applies the move to the board
    void executeMove(short fromCol, short fromRow, short toCol, short toRow);
    //quits and executes post-game code
    void endGame();
    //throws needed exceptions
    void handleExceptions();

    //returns all the possible cordinates where the king can move
    std::vector<std::pair<short, short>> KingPossibleMoves(short fromRow, short fromCol, char fromPieceColor) const;
    
    //prints current board to terminal (cout)
    void printBoard();
    //prints current board to file 
    void printToLog();
    void promotion(unsigned short pawnCol,unsigned short pawnRow);
    bool castling(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol, bool kingIsBlack);
    //returns 0 if tile is empty, otherwise returns piece id
    char getName(short row, short col) const;
    void EnPassant(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol);
    bool CheckOnEnPassant(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol);

    //returns board in a string

    std::string to_string(bool fixed_allignment);
    std::string getKing(bool requestColor);
};

#endif