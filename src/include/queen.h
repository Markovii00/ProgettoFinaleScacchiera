/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#ifndef QUEEN_H
#define QUEEN_H

#include "chessman.h"

class queen : public chessman
{
    public :
    queen(char type);
    ~queen();

    //Return true whether destination is legal for the queen's set of moves
    bool isLegalMove(unsigned short _startCol, unsigned short _startRow, unsigned short _destiCol, unsigned short _destiRow) override;
};

#endif