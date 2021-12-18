#ifndef BISHOP_H
#define BISHOP_H

#include "chessman.h"

class bishop : public chessman
{
    public :
    bishop(char type, short col, short row);
    void move(board& b, short mov_col, short mov_row) override;
};

#endif

