#ifndef ROOK_H
#define ROOK_H

#include "chessman.h"

class rook : public chessman
{
    private :
    bool first_move;

    public :
    rook();
    void move(board& b) override;

};
#endif