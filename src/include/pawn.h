/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#ifndef PAWN_H
#define PAWN_H

#include "chessman.h"
#include <cmath>

class pawn : public chessman
{
    private : 
    bool first_move;
    bool isWhite;

    public :
    pawn(char type);
    ~pawn();

    //Return true whether destination is legal for the pawn's set of moves
    bool isLegalMove(const coords& start, const coords& end) override;

    //Check part of the enPassant Conditions
    bool PartialEnPassantConditions(unsigned short& _startRow, unsigned short& _destiRow) const;

    //true if the piece has moved
    bool hasMoved(void) const;

    //true if the pawn is white
    bool is_White(void) const;

    //If the piece makes a move, remember to change the parameter first_move
    void setMoved(void);
};

#endif