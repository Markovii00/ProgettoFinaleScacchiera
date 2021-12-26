/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#ifndef ROOK_H
#define ROOK_H

#include "chessman.h"

class rook : public chessman
{
    private :
    bool first_move;

    public :
    rook(char type);
    ~rook();

    //Return true whether destination is legal for the rook's set of moves
    bool isLegalMove(unsigned short _startCol, unsigned short _startRow, unsigned short _destiCol, unsigned short _destiRow) override;

    //true if the piece has moved
    bool hasMoved(void) const ;

    //If the piece makes a move, remember to change the parameter first_move
    void setMoved(void);
};

#endif