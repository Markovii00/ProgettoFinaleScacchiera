#ifndef KING_H
#define KING_H

#include "chessman.h"

class king : public chessman
{
    private :

    public :
    void move(board& b) override;

};
#endif