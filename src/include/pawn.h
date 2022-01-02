/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#ifndef PAWN_H
#define PAWN_H

#include "chessman.h"

class pawn : public chessman
{
    private : 
    bool first_move;
    bool isWhite;

    public :
    pawn(char type);
    ~pawn();

    //Return true whether destination is legal for the pawn's set of moves
    bool isLegalMove(unsigned short _startRow, unsigned short _startCol, unsigned short _destiRow, unsigned short _destiCol) override;

    //true if the piece has moved
    bool hasMoved(void) const;

    //true if the pawn is white
    bool is_White(void) const;

    //If the piece makes a move, remember to change the parameter first_move
    void setMoved(void);
};

#endif