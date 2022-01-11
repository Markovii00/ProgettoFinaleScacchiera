/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#ifndef KNIGHT_H
#define KNIGHT_H

#include "chessman.h"
#include <cmath>

class knight : public chessman
{
    public:
    knight(char type);
    ~knight();

    //Return true whether destination is legal for the knight's set of moves
    bool isLegalMove(const coords& start, const coords& end) override;
};


#endif