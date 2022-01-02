/**
 * @author Alessandro _Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#include "include/queen.h"

queen::queen(char type) : chessman(type) {}

queen::~queen() {}

bool queen::isLegalMove(unsigned short _startRow, unsigned short _startCol, unsigned short _destiRow, unsigned short _destiCol)
{
    short dRow = _destiRow - _startRow;
    short dCol = _destiCol - _startCol;

    return (dRow == dCol) || (dCol == 0) || (dRow == 0);
}