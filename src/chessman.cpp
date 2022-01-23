/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#include <iostream>
#include "../include/chessman.h"

chessman::chessman(char p, coords pos, bool side) : piece{p}, position{pos}, set{side} { first_move = false; }

chessman::~chessman() = default;

char chessman::getChar() const { return piece; }
coords chessman::getPosition(void) const { return position; }
bool chessman::hasMoved(void) const { return first_move; }
bool chessman::getSet() const { return set; }

void chessman::setPosition(const coords& newPos) { position = newPos; }
void chessman::setMoved(void) { first_move = true; }
void chessman::unsetMoved(void) { first_move = false; }

king::king(char p, coords pos, bool side) : chessman(p, pos, side) {}

bool king::isLegalMove(const coords& start, const coords& end) const {
    if(position != start)
        return false;

    short dRow = end.first - start.first;
    short dCol = end.second - start.second;

    if(!first_move)
        return (abs(dCol) == 2 && dRow == 0) || ((dCol == 0) && (abs(dRow) == 1)) || ((abs(dCol) == 1) && (abs(dRow) == 1)) || (abs(dCol) == 1 && dRow == 0);

    return ((dCol == 0) && (abs(dRow) == 1)) || ((abs(dCol) == 1) && (abs(dRow) == 1)) || (abs(dCol) == 1 && dRow == 0);
}

std::vector<coords> king::possibleMoves(void) const {
    std::vector<coords> ret_;
    for (short i = -1; i < 2; i++) {
        for (short j = -1; j < 2; j++) {
            if (i == 0 && j == 0)
                continue;
            if(position.first + i >= 0 && position.first + i < 8 && position.second + j >= 0 && position.second + j < 8 ) 
                ret_.push_back(std::make_pair(position.first + i, position.second + j));
        }
    }
    if(!first_move) { 
        ret_.push_back(std::make_pair(position.first, position.second + 2));
        ret_.push_back(std::make_pair(position.first, position.second - 2));
    }

    return ret_;
}

queen::queen(char p, coords pos, bool side) : chessman(p, pos, side) {}

bool queen::isLegalMove(const coords& start, const coords& end) const {
    if(position != start)
        return false;

    short dRow = end.first - start.first;
    short dCol = end.second - start.second;

    return (abs(dRow) == abs(dCol)) || (dCol == 0 && dRow != 0) || (dRow == 0 && dCol != 0);

}

std::vector<coords> queen::possibleMoves(void) const {
    std::vector<coords> ret_;
    short counter = 1;

    bool Nord = true;
    bool Sud = true;
    bool East = true;
    bool West = true;
    bool Nord_East = true;
    bool Nord_West = true;
    bool Sud_West = true;
    bool Sud_East = true;

    while (counter < 8 && (Nord || Sud || East || West || Nord_East || Nord_West || Sud_East || Sud_West)) {
        if(Nord) Nord = position.first - counter >= 0;
        if(Sud) Sud = position.first + counter < 8;
        if(East) East = position.second + counter < 8;
        if(West) West = position.second - counter >= 0;
        if(Nord_East) Nord_East = position.first - counter >= 0 && position.second + counter < 8;
        if(Nord_West) Nord_West = position.first - counter >= 0 && position.second - counter >= 0;
        if(Sud_East) Sud_East = position.first + counter < 8 && position.second + counter < 8;
        if(Sud_West) Sud_West = position.first + counter < 8 && position.second - counter >= 0;

        if(Nord) ret_.push_back(std::make_pair(position.first - counter, position.second));
        if(Sud) ret_.push_back(std::make_pair(position.first + counter, position.second));
        if(East) ret_.push_back(std::make_pair(position.first,  position.second + counter));
        if(West) ret_.push_back(std::make_pair(position.first, position.second - counter));
        if(Nord_East) ret_.push_back(std::make_pair(position.first - counter, position.second + counter));
        if(Nord_West) ret_.push_back(std::make_pair(position.first - counter, position.second - counter));
        if(Sud_East) ret_.push_back(std::make_pair(position.first + counter, position.second + counter));
        if(Sud_West) ret_.push_back(std::make_pair(position.first + counter, position.second - counter));

        counter++;
    }
    return ret_;
}

rook::rook(char p, coords pos, bool side) : chessman(p, pos, side) {}

bool rook::isLegalMove(const coords& start, const coords& end) const {
    if(position != start)
        return false;

    return  (start.first == end.first && start.second != end.second) || (start.first != end.first && start.second == end.second);
}

std::vector<coords> rook::possibleMoves(void) const {
    std::vector<coords> ret_;
    short counter = 1;

    bool Nord = true;
    bool Sud = true;
    bool East = true;
    bool West = true;

    while (counter < 8 && (Nord || Sud || East || West)) {
        if(Nord) Nord = position.first - counter >= 0;
        if(Sud) Sud = position.first + counter < 8;
        if(East) East = position.second + counter < 8;
        if(West) West = position.second - counter >= 0;

        if(Nord) ret_.push_back(std::make_pair(position.first - counter, position.second));
        if(Sud) ret_.push_back(std::make_pair(position.first + counter, position.second));
        if(East) ret_.push_back(std::make_pair(position.first,  position.second + counter));
        if(West) ret_.push_back(std::make_pair(position.first, position.second - counter));

        counter++;
    }
    return ret_;
}

pawn::pawn(char p, coords pos, bool side) : chessman(p, pos, side) {}

bool pawn::isLegalMove(const coords& start, const coords& end) const {
    if(position != start)
        return false;

    short dRow = end.first - start.first;
    short dCol = end.second - start.second;
    
    if (set && !first_move)
        return ((dCol == 0) && (dRow == 2)) || ((dCol == 0) && (dRow == 1)) || ((dRow == 1) && (abs(dCol) == 1));
    else if (set && first_move)
        return ((dCol == 0) && (dRow == 1)) || ((dRow == 1) && (abs(dCol) == 1));
    else if (!set && !first_move)
        return ((dCol == 0) && (dRow == -2)) || ((dCol == 0) && (dRow == -1)) || ((dRow == -1) && (abs(dCol) == 1));
    else if (!set && first_move)
        return ((dCol == 0) && (dRow == -1)) || ((dRow == -1) && (abs(dCol) == 1));
    
    return false;
}

std::vector<coords> pawn::possibleMoves(void) const {
    std::vector<coords> ret_;
    
    if (set) {
        ret_.push_back(std::make_pair(position.first + 1, position.second));
        ret_.push_back(std::make_pair(position.first + 1, position.second + 1));
        ret_.push_back(std::make_pair(position.first + 1, position.second - 1));

        if(!first_move)
            ret_.push_back(std::make_pair(position.first + 2, position.second));
    }
    else {
        ret_.push_back(std::make_pair(position.first - 1, position.second));
        ret_.push_back(std::make_pair(position.first - 1, position.second + 1));
        ret_.push_back(std::make_pair(position.first - 1, position.second - 1));

        if(!first_move)
            ret_.push_back(std::make_pair(position.first - 2, position.second));
    }
    
    return ret_;
}

knight::knight(char p, coords pos, bool side) : chessman(p, pos, side) {}

bool knight::isLegalMove(const coords& start, const coords& end) const {
    if(position != start)
        return false;

    short dRow = end.first - start.first;
    short dCol = end.second - start.second;

    return ((abs(dRow) == 2) && (abs(dCol) == 1)) || ((abs(dRow) == 1) && (abs(dCol) == 2));
}

std::vector<coords> knight::possibleMoves(void) const {
    std::vector<coords> ret_;
    
    if(position.first + 2 < 8 && position.second + 1 < 8)
        ret_.push_back(std::make_pair(position.first + 2, position.second + 1));
    if(position.first + 2 < 8 && position.second - 1 >= 0)
        ret_.push_back(std::make_pair(position.first + 2, position.second - 1));
    if(position.first - 2 >= 0 && position.second + 1 < 8)
        ret_.push_back(std::make_pair(position.first - 2, position.second + 1));
    if(position.first - 2 >= 0 && position.second - 1 >= 0)
        ret_.push_back(std::make_pair(position.first - 2, position.second - 1));
    if(position.first + 1 < 8 && position.second + 2 < 8)
        ret_.push_back(std::make_pair(position.first + 1, position.second + 2));
    if(position.first + 1 < 8 && position.second - 2 >= 0)
        ret_.push_back(std::make_pair(position.first + 1, position.second - 2));
    if(position.first - 1 >= 0 && position.second + 2 < 8)
        ret_.push_back(std::make_pair(position.first - 1, position.second + 2));
    if(position.first - 1 >= 0 && position.second - 2 >= 0)
        ret_.push_back(std::make_pair(position.first - 1, position.second - 2));

    return ret_;
}

bishop::bishop(char p, coords pos, bool side) : chessman(p, pos, side) {}

bool bishop::isLegalMove(const coords& start, const coords& end) const {
    if(position != start)
        return false;

    short dRow = end.first - start.first;
    short dCol = end.second - start.second;

    return abs(dCol) == abs(dRow);
}

std::vector<coords> bishop::possibleMoves(void) const {
    std::vector<coords> ret_;
    short counter = 1;

    bool Nord_East = true;
    bool Nord_West = true;
    bool Sud_West = true;
    bool Sud_East = true;

    while (counter < 8 && (Nord_East || Nord_West || Sud_East || Sud_West)) {
        if(Nord_East) Nord_East = position.first - counter >= 0 && position.second + counter < 8;
        if(Nord_West) Nord_West = position.first - counter >= 0 && position.second - counter >= 0;
        if(Sud_East) Sud_East = position.first + counter < 8 && position.second + counter < 8;
        if(Sud_West) Sud_West = position.first + counter < 8 && position.second - counter >= 0;

        if(Nord_East) ret_.push_back(std::make_pair(position.first - counter, position.second + counter));
        if(Nord_West) ret_.push_back(std::make_pair(position.first - counter, position.second - counter));
        if(Sud_East) ret_.push_back(std::make_pair(position.first + counter, position.second + counter));
        if(Sud_West) ret_.push_back(std::make_pair(position.first + counter, position.second - counter));

        counter++;
    }
    return ret_;
}

emptyTile::emptyTile(char p, coords pos, bool side) : chessman(p, pos, side) {}

bool emptyTile::isLegalMove(const coords& start, const coords& end) const { return true; }

std::vector<coords> emptyTile::possibleMoves(void) const {
    std::vector<coords> ret_;
    return ret_;
}



