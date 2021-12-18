#ifndef QUEEN_H
#define QUEEN_H

#include "chessman.h"

class queen : public chessman
{

    public :
    void move(board& b) override;

};
#endif