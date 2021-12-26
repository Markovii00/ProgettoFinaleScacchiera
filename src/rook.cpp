#ifndef ROOK_CPP
#define ROOK_CPP

#include "include/rook.h"

rook::rook(char type, short _c, short _r) : chessman(type, _c, _r) { first_move = false; }

rook::~rook() {}

void rook::move(board& b, short mov_col, short mov_row) {
    
    
}

//Return true whether destination is legal for the rook's set of moves
bool rook::isLegalMove(short mov_col, short mov_row) {
    //XOR (exclusive or)
    return ((mov_col == col) && !(mov_row == row)) || (!(mov_col == col) && (mov_row == row));
}


#endif