/**
 * @file chessman.hpp
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 * @brief 
 * 
 */

#ifndef CHESSMAN_HPP
#define CHESSMAN_HPP

#include "board.hpp"

class chessman
{
protected:
    char piece;
    short col, row;

public:
    chessman();

    virtual void move(board& b) = 0;

    chessman(const chessman& other) = delete;
	chessman& operator=(const chessman& other) = delete;
};


#endif