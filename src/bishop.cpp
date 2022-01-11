/**
 * @author Alessandro _Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#include "include/bishop.h"

bishop::bishop(char type) : chessman(type) {}

bishop::~bishop() {}

bool bishop::isLegalMove(const coords& start, const coords& end)
{
    return abs(end.second - start.second) == abs(end.first - start.first);
}
