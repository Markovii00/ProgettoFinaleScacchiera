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
    bool isLegalMove(const coords& start, const coords& end) override;
};

#endif