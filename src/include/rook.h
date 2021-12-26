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
    rook(char type, short _c, short _r);
    ~rook();
    void move(board& b, short mov_col, short mov_row) override;
    bool isLegalMove(short mov_col, short mov_row) override;

};

#endif