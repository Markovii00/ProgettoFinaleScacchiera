/**
 * @author Alessandro _Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#ifndef KING_H
#define KING_H

#include "chessman.h"
#include <cmath>

class king : public chessman
{
    private:
    bool first_move;

    public:
    king(char type);
    ~king();

    //Return true whether destination is legal for the king's set of moves
    bool isLegalMove(unsigned short _startCol, unsigned short _startRow, unsigned short _destiCol, unsigned short _destiRow) override;

    //true if the piece has moved
    bool hasMoved(void) const;

    //Usefull to set first_move if a piece makes a first move
    void setMoved(void);
};

#endif