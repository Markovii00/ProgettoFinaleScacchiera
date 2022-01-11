/**
 * @author Alessandro _Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#include "include/knight.h"

knight::knight(char type) : chessman(type) {}

knight::~knight() {}

bool knight::isLegalMove(const coords& start, const coords& end)
{
    short dRow = end.first - start.first;
    short dCol = end.second - start.second;

    return ((abs(dRow) == 2) && (abs(dCol) == 1)) || ((abs(dRow) == 1) && (abs(dCol) == 2));
}