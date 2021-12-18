#ifndef QUEEN_H
#define QUEEN_H

#include "chessman.h"

class queen : public chessman
{
    public :
    queen(char type);
    void move(board& b, short mov_col, short mov_row) override;

};
#endif