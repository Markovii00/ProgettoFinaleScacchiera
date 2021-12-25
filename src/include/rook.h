/**
 * @file rook.h
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 * 
 */

#ifndef ROOK_H
#define ROOK_H

#include "chessman.h"

class rook : public chessman
{
    private :
    bool first_move;

    public :
    rook(char p, short col, short row, char type, short _c, short _r);
    ~rook();
    void move(board& b, short mov_col, short mov_row) override;

};

rook::rook(char p, short col, short row, char type, short _c, short _r) : chessman(p, col, row) {
    piece = type;
    row = _r;
    col = _c;
    first_move = false;
}

void rook::move(board& b, short mov_col, short mov_row) {

}
#endif