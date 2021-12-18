#ifndef ROOK_H
#define ROOK_H

#include "chessman.h"

class rook : public chessman
{
    private :

    public :
    void move(board& b) override;

};
#endif