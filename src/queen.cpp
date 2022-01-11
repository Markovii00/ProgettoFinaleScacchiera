/**
 * @author Alessandro _Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#include "include/queen.h"

queen::queen(char type) : chessman(type) {}

queen::~queen() {}

bool queen::isLegalMove(const coords& start, const coords& end)
{
    short dRow = end.first - start.first;
    short dCol = end.second - start.second;

    return (abs(dRow) == abs(dCol)) || (dCol == 0 && dRow != 0) || (dRow == 0 && dCol != 0);
}