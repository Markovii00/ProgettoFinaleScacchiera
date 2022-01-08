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
     * @brief   Look all the possible mechanics in chees game
     * 
     * @param   pieceToMoveColor black or white turn, initial and final coordinates 
     * @return  std::pair<bool,bool> :
                    -(false, false) game is over (Check mate), or tie
                    -(true, true)   move went smoothly
                    -(true, false)  king in check, move the king
                    -(false, true)  error in accettable move, legalMove, clearPath.  
     */
    std::pair<bool,bool> move(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol, bool& pieceToMoveColor);

    /**
     * @brief   When all the condition are checked the move action is here applied; updating all the involved pointers
     * 
     * @param  /initial and final coordinates
     * @return void                
     */
    void executeMove(unsigned short fromCol,unsigned short fromRow,unsigned short toCol,unsigned short toRow);


    //returns true if the king is in a safe position
    bool kingInCheck(bool requestColor);
    bool kingInCheck(std::pair<unsigned short, unsigned short>, bool requestColor);
    
    //quits and executes post-game code
    bool endGame();
    //throws needed exceptions
    void handleExceptions();

    //returns all the possible cordinates where the king can move
    std::vector<std::pair<unsigned short,unsigned short>> KingPossibleMoves(unsigned short fromRow, unsigned short fromCol, char fromPieceid) const;
    
    //prints current board to terminal (cout)
    void printBoard();
    //prints current board to file 
    void printToLog();
    void promotion(unsigned short pawnCol,unsigned short pawnRow);
    bool castling(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol, bool kingIsBlack);
    //returns 0 if tile is empty, otherwise returns piece id
    char getName(unsigned short row, unsigned short col) const;
    void EnPassant(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol);
    bool CheckOnEnPassant(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol);

    //returns board in a string

    std::string to_string(bool fixed_allignment);
    std::string getKing(bool requestColor);

    private:
    chessman* chessboard[8][8];
    unsigned short moveCounter;
    bool possibleEnPassant;
    std::pair<unsigned short, unsigned short> freezeCordinateEnPassant;
    std::vector<std::pair<unsigned short, unsigned short>> whiteSet;
    std::vector<std::pair<unsigned short, unsigned short>> blackSet;

    bool movePawn(unsigned short fromCol, unsigned short fromRow, unsigned short toCol, unsigned short toRow, bool fromPieceColor);

    bool checkTie();

    template<typename Type>
    bool is(const chessman &data);

    template<typename Type>
    std::pair<unsigned short, unsigned short> search(bool& requestColor) const;

    template<typename Type>
    bool find(bool& requestColor) const;

    bool isBlack(char& request) const;
    
    bool isVertical(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol) const;
    bool isHorizontal(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol) const;
    bool isDiagonal(unsigned short& fromRow, unsigned short& fromCol, unsigned short& toRow, unsigned short& toCol) const;
};

#endif