/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#include "include/rook.h"

rook::rook(char type) : chessman(type) { first_move = false; }

rook::~rook() {}

bool rook::isLegalMove(unsigned short _startCol, unsigned short _startRow, unsigned short _destiCol, unsigned short _destiRow)
{
    //XOR (exclusive or)
    return ((_destiCol == _startCol) && !(_destiRow == _startRow)) || (!(_destiCol == _startCol) && (_destiRow == _startRow));
}

bool rook::hasMoved(void) const { return first_move; }

void rook::setMoved(void) { first_move = true; }
