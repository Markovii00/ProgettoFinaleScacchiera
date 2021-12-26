/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#include "include/king.h"

king::king(char type) : chessman(type) { first_move = false; }

king::~king() {}

bool king::isLegalMove(unsigned short _startCol, unsigned short _startRow, unsigned short _destiCol, unsigned short _destiRow)
{
    short dRow = _destiRow - _startRow;
    short dCol = _destiCol - _startCol;

    return ((dCol == 0) && (abs(dRow) == 1)) || ((abs(dCol) == 1) && (abs(dRow) == 1)) || ((dCol == 1) && (abs(dRow) == 0));
}

bool king::hasMoved(void) const { return first_move; }

void king::setMoved(void) { first_move = true; }