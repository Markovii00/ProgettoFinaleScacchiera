#ifndef KING_H
#define KING_H

#include "chessman.h"

class king : public chessman
{
    private :
    bool first_move;

    public :
    king(char type);
    void move(board& b, short mov_col, short mov_row) override;
    void castling();
};
#endif