/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#ifndef BISHOP_H
#define BISHOP_H

#include "chessman.h"

class bishop : public chessman
{
    public :
    bishop(char type);
    ~bishop();

    //Return true whether destination is legal for the bishop's set of moves
    bool isLegalMove(const coords& start, const coords& end) override;
};

#endif

