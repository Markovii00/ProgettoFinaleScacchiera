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

typedef std::pair<unsigned short, unsigned short> coords;

class board
{
    public:
    board(std::vector<chessman*> copy, std::pair<coords, coords> lastMC); 
    board(void);
    ~board(void);

    /**
     * @brief   Check if there is a clear way in the board 
     * 
     * @param fromPieceId piece to check the path and initial and final coordinates
     * @return true if there is a clear way                 
     */
    bool clearPath(coords& start, coords& end, char& fromPieceId) const;

    /**
     * @brief   Check if the coordinates are within the board limits and whether in (toRow, toCol) there is either an opponent piece
     *          or empty tile
     * 
     * @param fromPieceId piece to check and initial and final coordinates
     * @return true if all the conditions in @brief are true                
     */
    bool acceptableMove(coords start, coords end, char& fromPieceId, bool& fromPieceColor) const;

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
    std::pair<bool, bool> move(coords& start, coords& end, bool& pieceToMoveColor);

    /**
     * @brief   When all the condition are checked the move action is here applied; updating all the involved pointers
     * 
     * @param  /initial and final coordinates
     * @return void                
     */
    void executeMove(coords start, coords end);

    /**
     * @brief   If the king is in check returns the coordinates of the first menacing chesspiece
     * 
     * @param  /initial and final coordinates
     * @return 100/100 if not in check                
     */
    bool kingInCheck(const coords& kingCords, const bool& requestColor) const;
    
    //quits and executes post-game code
    bool endGame();
    //throws needed exceptions
    void handleExceptions();

    bool checkKing(coords& start, coords& end, bool& fromPieceColor);

    //returns all the possible cordinates where the king can move
    std::vector<coords> KingPossibleMoves(coords& kCords, bool& fromPieceColor) const;
    
    /**
     * @brief When called prints the board to standard out 
     */
    void printBoard();
    

    /**
     * @brief executes all checks and executes the castling move
     * 
     * @param start initial coordinates
     * @param end   final coordinates
     * @param kingIsBlack  color of the king executing the castling 
     * @return true move executed
     */
    std::pair<bool, coords> isCastling(const coords& start, const coords& end) const;

    /**
     * @brief executres all checks and executes  en passant move
     * 
     * @param start initial coordinates
     * @param end   final coordinates
     * @param fromPieceColor  color of the piece to move
     * @return true  move executed
     */
    std::pair<bool, coords> isEnpassant(coords& start, coords& end);


    //returns 0 if tile is empty, otherwise returns piece id
    char getName(unsigned short row, unsigned short col) const;
    void applyEnPassant(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol);
    bool CheckOnEnPassant(unsigned short fromRow, unsigned short fromCol, unsigned short toRow, unsigned short toCol);
    void promotion(unsigned short pawnCol,unsigned short pawnRow);

    //returns board in a string

    std::string to_string(bool fixed_allignment);
    std::string getKing(bool requestColor);

    std::vector<coords> getAllMoves(const coords& _pos) const;

    private:
    chessman* chessboard[8][8];
    std::vector<coords> whiteSet;
    std::vector<coords> blackSet;
    std::pair<coords, coords> lastMoveCoords;

    unsigned short moveCounter;

    bool movePawn(unsigned short fromCol, unsigned short fromRow, unsigned short toCol, unsigned short toRow, bool fromPieceColor);
    void do_castling(coords& start, coords& end, coords& rook_to_move);
    void do_enpassant(coords& start, coords& end, coords& pawn_to_be_eaten);

    std::vector<chessman*> copy_board();

    bool draw_for_pieces(void) const;

    template<typename Type>
    coords search(bool& requestColor) const;

    template<typename Type>
    bool find(bool& requestColor) const;

    bool isBlack(const char& request) const;

    bool isEatable(coords, bool toSacrificeColor);
    
    bool isVertical(coords& start, coords& end) const;
    bool isHorizontal(coords& start, coords& end) const;
    bool isDiagonal(coords& start, coords& end) const;
};

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