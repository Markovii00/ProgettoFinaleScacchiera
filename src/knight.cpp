/**
 * @author Alessandro _Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#include "include/knight.h"

knight::knight(char type) : chessman(type) {}

knight::~knight() {}

bool knight::isLegalMove(unsigned short _startCol, unsigned short _startRow, unsigned short _destiCol, unsigned short _destiRow)
{
    short dRow = _destiRow - _startRow;
    short dCol = _destiCol - _startCol;

    return ((abs(dRow) == 2) && (abs(dCol) == 1)) || ((abs(dRow) == 1) && (abs(dCol) == 2));
}