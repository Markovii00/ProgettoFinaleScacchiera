#ifndef KNIGHT_H
#define KNIGHT_H

#include "chessman.h"

class knight : public chessman
{
    private:
    
    public:
    void move(board& b) override;

};


#endif