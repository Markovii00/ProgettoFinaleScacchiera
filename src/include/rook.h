#ifndef ROOK_H
#define ROOK_H

#include "chessman.h"

class rook : public chessman
{
    private :
    bool first_move;

    public :
    rook(char type, short col, short row);
    void move(board& b, short mov_col, short mov_row) override;

};
#endif