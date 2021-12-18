#ifndef ROOK_CPP
#define ROOK_CPP

#include "src/include/rook.h"

rook::rook(char type, short _c, short _r) {
    piece = type;
    row = _r;
    col = _c;
    first_move = false;
}

void rook::move(board& b, short mov_col, short mov_row) {
    
}


#endif