#ifndef KNIGHT_H
#define KNIGHT_H

#include "chessman.h"

class knight : public chessman
{
    public:
    knight(char type, short col, short row);
    void move(board& b, short mov_col, short mov_row) override;

};


#endif