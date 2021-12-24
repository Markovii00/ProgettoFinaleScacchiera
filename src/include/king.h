/**
 * @file king.h
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 * 
 */

#ifndef KING_H
#define KING_H

#include "chessman.h"

class king : public chessman
{
    private :
    bool first_move;

    public :
    king(char type);
    ~king();

    void move(board& b, short mov_col, short mov_row) override;
    bool isLegalMove(short mov_col, short mov_row) override;

    void alreadyMoved() const;

    void castling();
};
#endif