#ifndef CHESSMAN_CPP
#define CHESSMAN_CPP

#include "include/chessman.h"

chessman::chessman(char p, short c, short r) : piece{p}, col{c}, row{r} {}

chessman::~chessman() {}
 
short chessman::getRow() const { return row; }

short chessman::getCol() const { return col; }

char chessman::getChar() const { return piece; }

#endif