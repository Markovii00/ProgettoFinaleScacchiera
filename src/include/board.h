/**
*@file board.hpp
*@author  Francesco Colla - 2007968 (francesco.colla.2@studenti.unipd.it)
*@brief
*
*/

#ifndef BOARD_H
#define BOARD_H

#include "../chessman.cpp"
#include "../king.cpp"
#include "../pawn.cpp"
#include "../bishop.cpp"
#include "../queen.cpp"
#include "../rook.cpp"
#include "../knight.cpp"
#include <vector>
#include <utility>
#include <string>

class board
{
    public: 
    board();
    ~board();

    /**
     * @brief   Check if there is a clear way in the board 
     * 
     * @param fromPieceId piece to check the path and initial and final coordinates
     * @return true if there is a clear way                 
     */
    bool clearPath(unsigned short& fromCol, unsigned short& fromRow, unsigned short& toCol, unsigned short& toRow, char& fromPieceId) const;

    /**
     * @brief   Check if the coordinates are within the board limits and whether in (toRow, toCol) there is either an opponent piece
     *          or empty tile
     * 
     * @param fromPieceId piece to check and initial and final coordinates
     * @return true if all the conditions in @brief are true                
     */
    bool acceptableMove(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol, char& fromPieceId) const;


    /**
     * @brief   When all the condition are checked the move action is here applied; updating all the involved pointers
     * 
     * @param  /initial and final coordinates
     * @return void                
     */
    void executeMove(short fromCol, short fromRow, short toCol, short toRow);

    bool findKing();
  
   
    
    //returns true if the king is in a safe position
    bool kingInCheck(bool requestColor);
    bool kingInCheck(short col, short row, bool requestColor);
    
    bool move(unsigned short fromCol, unsigned short fromRow, unsigned short toCol, unsigned short toRow, char pieceToMoveColor);
    
   
    //applies the move to the board
    void executeMove(short fromCol, short fromRow, short toCol, short toRow);
    //quits and executes post-game code
    bool endGame();
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

    private:
    chessman* chessboard[8][8];
    bool isWhiteTurn;
    int maxMoves;
    bool possibleEnPassant;
    std::pair<short, short> freezeCordinateEnPassant;

    bool movePawn(unsigned short fromCol, unsigned short fromRow, unsigned short toCol, unsigned short toRow, bool fromPieceColor);

    bool isBlack(char& request) const;
    bool isPawn(char& pieceId) const;
    bool isKing(char& pieceId) const;
    bool isQueen(char& pieceId) const;
    bool isBishop(char& pieceId) const;
    bool isKnight(char& pieceId) const;
    bool isRook(char& pieceId) const;
    bool isVertical(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol) const;
    bool isHorizontal(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol) const;
    bool isDiagonal(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol) const;
};

#endif