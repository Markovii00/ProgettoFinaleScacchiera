/**
 * @file bishop.h
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 * 
 */

#ifndef BISHOP_H
#define BISHOP_H

#include "chessman.h"

class bishop : public chessman
{
    public :
    bishop(char type, short col, short row);
    ~bishop();
    void move(board& b, short mov_col, short mov_row) override;
    bool isLegalMove(short mov_col, short mov_row) override;
};

#endif

