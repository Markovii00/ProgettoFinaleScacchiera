#ifndef QUEEN_H
#define QUEEN_H

#include "chessman.h"

class queen : public chessman
{
    public :
    queen();
    void move(board& b) override;

};
#endif