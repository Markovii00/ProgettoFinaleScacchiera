/**
*@author  Alessandro Viespoli - 2009659 (alessandro.viespoli@studenti.unipd.it)
*
*/

#ifndef BOARD_H
#define BOARD_H

#include "../chessman.h"
#include <vector>
#include <utility>
#include <string>
#include <map>

typedef std::pair<short, short> coords;

class board {
public:
    board(std::vector<chessman*> copy, std::pair<coords, coords> lastMC); 
    board(void);
    ~board(void);

    std::pair<bool, bool> move(coords& start, coords& end, bool& pieceToMoveColor);

    void printBoard(void) const;
    std::string to_string(bool fixed_allignment) const;
    std::string boardToString(void) const;  

    std::vector<coords> getAllMoves(const coords& _pos) const;
    std::vector<coords> getWhiteSet(void) const;
    std::vector<coords> getBlackSet(void) const;

private:
    chessman* chessboard[8][8];

    //Checks for ties, Vector of board where short indicates n times board appears
    std::map<std::string, short> tables;
    //Makes more efficient KingInCheck, Verify Tie conditions and keeps updated chessman sets
    std::vector<coords> whiteSet;
    std::vector<coords> blackSet;
    //Keeps saved last made move, useful for enpassant
    std::pair<coords, coords> lastMoveCoords;
    //Counts moves number made without eating a chessman or moving a pawn
    int moveRule50;

    void do_castling(const coords& start, const coords& end, const coords& rook_to_move);
    void do_enpassant(const coords& start, const coords& end, const coords& pawn_to_be_eaten);

    //int number_possible_moves(const bool& fromPieceColor) const;

    //void insertBoardInMap(void);

    bool illegalMove(const coords& start, const coords& end,const char& fromPieceId, const bool& pieceToMoveColor, const bool& whoseturn) const;

    std::pair<bool, coords> isCastling(const coords& start, const coords& end) const;
    std::pair<bool, coords> isEnpassant(const coords& start, const coords& end) const;
    bool isPawnEating(const coords& start, const coords& end, const bool& fromPieceColor) const;
    //bool attemptMove(std::vector<coords>& _vet, const bool& colorPiece, const coords& _tempCoords) const;
    bool check_on_pawn(const coords& start, const coords& end, const char& fromPieceId, const bool& fromPieceColor) const;
    //bool isSafeMove(const coords& start, const coords& end, bool& pieceToMoveColor) const;

    //bool isPromotion(const coords& end, const bool& fromPieceColor) const;
    

    // -> void fakeExecuteMove(const coords& start, const coords& end, const int& typeMove, const coords& pawn_to_be_eaten, const coords& rook_to_move);

    void removeFromSet(const coords &coordsPieceEaten, const bool& pieceEaten);
    void updateCoordsInSet(const coords &start, const coords &end, const bool& pieceEaten);
    void executeMove(const coords& start, const coords& end);
    //int isTie(const bool& fromPieceColor) const;
    
    // eclissiamo void promotion(const coords& pawnPos, const bool& pawnColor);
    
    // bool checkKing(coords& start, coords& end, bool& fromPieceColor);

    
    //std::vector<coords> KingPossibleMoves(coords& kCords, bool& fromPieceColor) const;


    bool withinBoardLimits(const coords& end) const;
    bool acceptableMove(const coords& start, const coords& end, const char& fromPieceId, const bool& fromPieceColor) const;

    bool clearPath(const coords& start, const coords& end, const char& fromPieceId) const;
    bool kingInCheck(const coords& kingCords, const bool& requestColor) const;




    // da rivedere std::vector<chessman*> copy_board(void) const;

    //bool draw_for_pieces(void) const;

    bool isVertical(const coords& start, const coords& end) const;
    bool isHorizontal(const coords& start, const coords& end) const;
    bool isDiagonal(const coords& start, const coords& end) const;


    template<typename Type>
    coords search(const bool& requestColor) const;

    template<typename Type>
    bool find(const bool& requestColor) const;

    template<typename Type>
    int howManyAlive(const bool& requestColor) const; 
};

coords operator+(const coords& start, std::pair<short, short> offset);

template<typename Type>
bool is(chessman &data) {
    if( &data == nullptr ) return false;

    if (typeid(Type) == typeid(rook)) return tolower(data.getChar()) == 't';
    if (typeid(Type) == typeid(knight)) return tolower(data.getChar()) == 'c';
    if (typeid(Type) == typeid(bishop)) return tolower(data.getChar()) == 'a';
    if (typeid(Type) == typeid(king)) return tolower(data.getChar()) == 'r';
    if (typeid(Type) == typeid(queen)) return tolower(data.getChar()) == 'd';
    if (typeid(Type) == typeid(pawn)) return tolower(data.getChar()) == 'p';
    if (typeid(Type) == typeid(emptyTile)) return tolower(data.getChar()) == ' ';

    return false;
}

#endif