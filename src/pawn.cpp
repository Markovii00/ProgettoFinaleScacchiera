/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#include "include/pawn.h"

pawn::pawn(char type) : chessman(type)
{
    first_move = false;
    if (type >= 'A' && type <= 'Z')
    {
        isWhite = false;
    }
    else
        isWhite = true;
}

pawn::~pawn() {}

bool pawn::isLegalMove(const coords& start, const coords& end)
{
    short dRow = end.first - start.first;
    short dCol = end.second - start.second;
    
    if (isWhite && !first_move)
    {
        return ((dCol == 0) && (dRow == -2)) || ((dCol == 0) && (dRow == -1)) || ((dRow == -1) && (abs(dCol) == 1));
    }
    else if (isWhite && first_move)
    {
        return ((dCol == 0) && (dRow == -1)) || ((dRow == -1) && (abs(dCol) == 1));
    }
    else if (!isWhite && !first_move)
    {
        return ((dCol == 0) && (dRow == 2)) || ((dCol == 0) && (dRow == 1)) || ((dRow == 1) && (abs(dCol) == 1));
    }
    else if (!isWhite && first_move)
    {
        return ((dCol == 0) && (dRow == 1)) || ((dRow == 1) && (abs(dCol) == 1));
    }

    return false;
}

bool pawn::PartialEnPassantConditions(unsigned short& _startRow, unsigned short& _destiRow) const
{
    return (abs(_destiRow - _startRow) == 2) && (first_move == false);
}

bool pawn::is_White(void) const { return isWhite; }
bool pawn::hasMoved(void) const { return first_move; }
void pawn::setMoved(void) { first_move = true; }