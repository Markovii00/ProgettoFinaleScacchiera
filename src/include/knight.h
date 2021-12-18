#ifndef KNIGHT_H
#define KNIGHT_H

#include "chessman.h"

class knight : public chessman
{
    public:
    knight();
    void move(board& b) override;

};


#endif