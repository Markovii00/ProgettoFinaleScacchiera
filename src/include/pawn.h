#ifndef PAWN_H
#define PAWN_H

#include "chessman.h"

class pawn : public chessman
{
    private :

    public :
    void move(board& b) override;

};
#endif