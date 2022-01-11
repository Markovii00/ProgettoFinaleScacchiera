/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#include "include/rook.h"

rook::rook(char type) : chessman(type) { first_move = false; }

rook::~rook() {}

bool rook::isLegalMove(const coords& start, const coords& end)
{
    return  (start.first == end.first && start.second != end.second) || (start.first != end.first && start.second == end.second);
}

bool rook::hasMoved(void) const { return first_move; }

void rook::setMoved(void) { first_move = true; }
