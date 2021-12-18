#ifndef BISHOP_H
#define BISHOP_H

#include "chessman.h"

class bishop : public chessman
{
    public :
    bishop();
    void move(board& b) override;
};

#endif

