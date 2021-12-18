#ifndef PAWN_H
#define PAWN_H

#include "chessman.h"

class pawn : public chessman
{
    private : 
    bool first_move;
    bool promotion;

    public :
    pawn();
    void move(board& b) override;
    bool enPassant() const;

};
#endif