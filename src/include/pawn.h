/**
 * @file pawn.h
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 * 
 */

#ifndef PAWN_H
#define PAWN_H

#include "chessman.h"

class pawn : public chessman
{
    private : 
    bool first_move;
    bool promotion;

    public :
    pawn(char type, short col, short row);
    ~pawn();
    void move(board& b, short mov_col, short mov_row) override;
    bool enPassant() const;

};
#endif