/**
*@author  Alessandro Viespoli - 2009659 (alessandro.viespoli@studenti.unipd.it)
*
*/

#ifndef BOARD_H
#define BOARD_H

#include "chessman.h"
#include <vector>
#include <utility>
#include <string>
#include <map>
#include <typeinfo>

typedef std::pair<short, short> coords;

class board {
public:
    board(void);
    ~board(void);

    /**
     * @brief Check all possible errors, conditions and rules of the chess game, then apply the move
     * @param start selected position in board
     * @param end final destination
     * @param whoseturn true if is the Black turn, otherwise is White turn
     * @param attemptMove   generally false, except when used in method getSetPossibleMoves
     * @param bypassDraftAsk   generally false, except when used to skip Threefold draft condition
     * @return first parameter : true -> move executed. Second parameter in combo with true state :
     *                              # 1 nothing is required
     *                              # 2 promotion is required
     *                              # 3 CheckMate, game is over
     *         first parameter : false -> move NOT executed. Second parameter in combo with false state :
     *                              # 1 the inserted move is not valid
     *                              # 2 king is still under check, after the attempted move
     *                              # 3 ask for draw, only when the threefold repetion rule occurs
     *                              # 4 the game is over for pieces number draw
     *                              # 5 the game is over for stalled game
     *                              # 6 the game is over for moveRule50 draw
     */
    std::pair<bool, int> move(coords &start, coords &end, bool whoseturn, bool attemptMove, bool bypassDraftAsk);

    /**
     * @brief ask for all the possible and applicable moves a set can make
     * @param setColor   true for BlackSet, false for WhiteSet
     * @return vector of pair of coords. Inside the pair there are the start and final position
     */
    std::vector<std::pair<coords, coords>> getSetPossibleMoves(bool setColor);

    /**
     * @brief recive a character, which is the piece the pawn is promoted. Apply the changes to the board
     * @param promotionChess might be a queen, rook, knight or bishop 
     * @param pawnColor true for black piece, false for white piece
     * @return first parameter: true  -> promotion executed with success
     *                          false -> promotion not done: invalid argument or promotion not asked
     *
     *         second parameter: true  -> enemy king not in mate, the game will resume
     *                           false -> enemy king in mate, game ends
     */
    std::pair<bool, bool> promotion(short promotionChess, bool pawnColor);

    /**
     * @brief print the board
     */
    void printBoard(void) const;

     /**
     * @return a vector of all the white pieces coordinates
     */
    std::vector<coords> getWhiteSet(void) const;

    /**
     * @return a vector of all the black pieces coordinates
     */
    std::vector<coords> getBlackSet(void) const;

    /**
    * @return board as a string
    */
    std::string to_string() const;

private:
    chessman* chessboard[8][8];
    bool needPromotion = false;
    coords toBePromoted;
    int moveRule50 = 0;

    std::map<std::string, short> tables;
    std::vector<coords> whiteSet;
    std::vector<coords> blackSet;
    std::pair<coords, coords> lastMoveCoords;

    void executeMove(const coords& start, const coords& end);
    void do_castling(const coords& start, const coords& end, const coords& rook_to_move);
    void do_enpassant(const coords& start, const coords& end, const coords& pawn_to_be_eaten);
    void updateCoordsInSet(const coords &start, const coords &end, const bool& pieceEaten);
    void removeFromSet(const coords &coordsPieceEaten, const bool& pieceEaten);
    void insertBoardInMap(void);
    std::string boardToString(void) const;

    void undoMove(const coords &start, const coords &end, const bool &fromPieceColor, const int &typeOfMove, char &pieceAtEnd, 
                  const int &old_moveRule50, const std::pair<coords, coords> &old_lastMovedCoords, const std::pair<bool,bool> &oldMovedVal);
    void removeBoardFromMap(void);
    void addCoordsInSet(const coords &position, const bool &colorSet);
    
    bool illegalMove(const coords& start, const coords& end,const char& fromPieceId, const bool& pieceToMoveColor, const bool& whoseturn) const;
    bool acceptableMove(const coords& start, const coords& end, const char& fromPieceId, const bool& fromPieceColor) const;
    bool withinBoardLimits(const coords& end) const;
    bool clearPath(const coords& start, const coords& end, const char& fromPieceId) const;

    bool kingInCheck(const coords &king_coordinates, const bool &requestColor) const;
    bool kingInMate(bool setToCheck, const coords &kingPos);

    std::pair<bool, coords> isEnpassant(const coords& start, const coords& end) const;
    std::pair<bool, coords> isCastling(const coords &start, const coords &end) const;

    bool isPawnEating(const coords& start, const coords& end, const bool& fromPieceColor) const;
    bool check_on_pawn(const coords& start, const coords& end, const char& fromPieceId, const bool& fromPieceColor) const;
    bool isPromotion(const coords &end, const bool &fromPieceColor) const;
    
    bool isVertical(const coords& start, const coords& end) const;
    bool isHorizontal(const coords& start, const coords& end) const;
    bool isDiagonal(const coords& start, const coords& end) const;

    std::pair<bool, int> isTie(bool& pieceToMoveColor);
    bool draw_for_pieces(void) const;

    template<typename Type>
    coords search(const bool& requestColor) const;

    template<typename Type>
    bool contains(const bool& requestColor) const;

    template<typename Type>
    int howManyAlive(const bool& requestColor) const;

    std::string to_string(bool fixed_allignment) const;

};

//Helper function
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