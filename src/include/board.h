/**
*@author  Alessandro Viespoli - 2009659 (alessandro.viespoli@studenti.unipd.it)
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

typedef std::pair<unsigned short, unsigned short> coords;

class board {
public:
    board(std::vector<chessman*> copy, std::pair<coords, coords> lastMC); 
    board(void);
    ~board(void);

    std::pair<bool, bool> move(coords& start, coords& end, bool& pieceToMoveColor);

    void printBoard(void) const;
    std::string to_string(bool fixed_allignment) const;

    std::vector<coords> getAllMoves(const coords& _pos) const;
    std::vector<coords> getWhiteSet(void) const;
    std::vector<coords> getBlackSet(void) const;


private:
    chessman* chessboard[8][8];
    std::vector<coords> whiteSet;
    std::vector<coords> blackSet;
    std::pair<coords, coords> lastMoveCoords;

    void do_castling(const coords& start, const coords& end, const coords& rook_to_move);
    void do_enpassant(const coords& start, const coords& end, const coords& pawn_to_be_eaten);

    int number_possible_moves(const bool& fromPieceColor);

    std::pair<bool, coords> isCastling(const coords& start, const coords& end) const;
    std::pair<bool, coords> isEnpassant(const coords& start, const coords& end) const;
    bool isPawnEating(const coords& start, const coords& end, bool& fromPieceColor) const;
    bool attemptMove(std::vector<coords>& _vet, const bool& colorPiece, const coords& _tempCoords) const;
    bool check_on_pawn(const coords start, const coords end, const char& fromPieceId, bool& fromPieceColor) const;
    
    void removeFromSet(const coords &coordsPieceEaten, const bool& pieceEaten);
    void updateCoordsInSet(const coords &start, const coords &end, const bool& pieceEaten); 

    
    char getName(unsigned short row, unsigned short col) const;
    void promotion(const coords& pawnPos);
     

    bool endGame();
    
    bool checkKing(coords& start, coords& end, bool& fromPieceColor);

    
    std::vector<coords> KingPossibleMoves(coords& kCords, bool& fromPieceColor) const;
    void executeMove(const coords& start, const coords& end);

  
    bool kingInCheck(const coords& kingCords, const bool& requestColor) const;
    bool clearPath(const coords& start, const coords& end, const char& fromPieceId) const;

    bool acceptableMove(const coords start, const coords end, char& fromPieceId, bool& fromPieceColor) const;

    std::vector<chessman*> copy_board(void) const;

    bool draw_for_pieces(void) const;

    bool isBlack(const char& request) const;
    bool isVertical(const coords& start, const coords& end) const;
    bool isHorizontal(const coords& start, const coords& end) const;
    bool isDiagonal(const coords& start, const coords& end) const;


    template<typename Type>
    coords search(bool& requestColor) const;

    template<typename Type>
    bool find(bool& requestColor) const;

    template<typename Type>
    int howManyAlive(bool& requestColor) const; 
};

coords operator+(const coords& start, std::pair<short, short> offset) {
    return std::make_pair(start.first + offset.first, start.second + offset.second);
}

template<typename Type>
bool is(chessman &data) {
    if( &data == NULL ) return false;

    if (typeid(Type) == typeid(rook)) return tolower(data.getChar()) == 't';
    if (typeid(Type) == typeid(knight)) return tolower(data.getChar()) == 'c';
    if (typeid(Type) == typeid(bishop)) return tolower(data.getChar()) == 'a';
    if (typeid(Type) == typeid(king)) return tolower(data.getChar()) == 'r';
    if (typeid(Type) == typeid(queen)) return tolower(data.getChar()) == 'd';
    if (typeid(Type) == typeid(pawn)) return tolower(data.getChar()) == 'p';

    return false;
}

#endif