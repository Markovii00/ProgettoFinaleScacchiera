/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#include "include/king.h"

king::king(char type) : chessman(type) { first_move = false; }

king::~king() {}

bool king::isLegalMove(const coords& start, const coords& end)
{
    short dRow = end.first - start.first;
    short dCol = end.second - start.second;
    if(!first_move) 
        return (abs(dRow == 2) && dCol == 0) || ((dCol == 0) && (abs(dRow) == 1)) || ((abs(dCol) == 1) && (abs(dRow) == 1)) || ((dCol == 1) && (abs(dRow) == 0));

    return ((dCol == 0) && (abs(dRow) == 1)) || ((abs(dCol) == 1) && (abs(dRow) == 1)) || ((dCol == 1) && (abs(dRow) == 0));
}

bool king::hasMoved(void) const { return first_move; }

void king::setMoved(void) { first_move = true; }