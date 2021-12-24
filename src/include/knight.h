/**
 * @file knight.h
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 * 
 */

#ifndef KNIGHT_H
#define KNIGHT_H

#include "chessman.h"

class knight : public chessman
{
    public:
    knight(char type, short col, short row);
    ~knight();
    void move(board& b, short mov_col, short mov_row) override;

};


#endif