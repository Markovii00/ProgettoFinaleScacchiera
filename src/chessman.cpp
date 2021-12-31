/**
 * @author Alessandro Viespoli - 2009659 (alessandro.viespoli@studentiunipd.it)
 */

#include "include/chessman.h"

chessman::chessman(char p) : piece{p} {}

chessman::~chessman() {

}

char chessman::getChar() const { return piece; }
