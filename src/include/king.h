#ifndef KING_H
#define KING_H

#include "chessman.h"

class king : public chessman
{
    private :
    bool first_move;

    public :
    king();
    void move(board& b) override;
    void castling();
};
#endif