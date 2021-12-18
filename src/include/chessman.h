/**
 * @file chessman.hpp
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 * @brief 
 * 
 */

#ifndef CHESSMAN_H
#define CHESSMAN_H

#include "board.h"

class chessman
{
    protected:
    char piece;
    short row, col;

    public:
    chessman();
    virtual void move(board& b, short mov_col, short mov_row) = 0;

    chessman(const chessman& other) = delete;
	chessman& operator=(const chessman& other) = delete;
};


#endif