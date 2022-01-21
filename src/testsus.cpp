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


    while(true) {
        b.printBoard();
        do {
            cout << "\n\nINSERT MOVE : ";
            getline(cin, input);
            regex_search(input, coordinates, input_filter);
        }while(!regex_match(input, input_filter));

        short *mosse = conversion(coordinates);
        start.first = *(mosse);
        start.second = *(mosse + 1);
        end.first = *(mosse + 2);
        end.second = *(mosse + 3);
        moveOutput = b.move(start, end, white, false);

        if (moveOutput.first == true) {
            if (moveOutput.second == false) {
                bool promotionRes = false;
                do {
                    short pedinaDaPromuovere;
                    cin >> pedinaDaPromuovere;
                    promotionRes = b.promotion(pedinaDaPromuovere, white);
                } while (!promotionRes);
            }
        }
            white = !white;
    }
     return 0;
}