#ifndef BISHOP_H
#define BISHOP_H

#include "chessman.h"

class bishop : public chessman
{
    private :

    public :
    void move(board& b) override;

};
#endif

#endif