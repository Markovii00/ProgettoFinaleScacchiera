#include <iostream>
#include <cctype>
#include <chrono>
#include <thread>
#include <string>
#include <algorithm>
#include <regex>
#include "board.h"

using namespace std;

short* conversion(smatch& m) 
{
    short fromCol = toupper(m.str(1)[0]) - 65;
    short fromRow = 8 - stoi(m.str(2));
    short toCol = toupper(m.str(3)[0]) - 65;
    short toRow = 8 - stoi(m.str(4));

    short ret[4] = {fromRow, fromCol, toRow, toCol};
    short *array = &ret[0];
    return array; 
}

int main(){
    string input;
    smatch coordinates;
    pair<bool,bool> mossaNonValida = make_pair(false, true);
    pair<bool,bool> partitaInCulo = make_pair(false, false);
    pair<bool,bool> mossaEseguita = make_pair(true, true);
    pair<bool,bool> moveOutput;

    regex input_filter("^([a-hA-H]){1}([1-8]){1} ([a-hA-H]){1}([1-8]){1}$");

    coords start;
    coords end;

    bool white = false;

    board b {};

    b.printBoard();
    return 0;
}