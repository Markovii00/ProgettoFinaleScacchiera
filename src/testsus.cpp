#include "include/board.h"
#include <iostream>
#include <cctype>
#include <chrono>
#include <thread>
#include <string>
#include <algorithm>
#include <regex>

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
    pair<unsigned short, unsigned short> start;
    pair<unsigned short, unsigned short> end;
    bool black = true;

    board b {};

    while(true){
        b.printBoard();
        if(black == false){
            cout << "turno dei bianchi" << endl;
        }
        else if(black == true){
            cout << "turno dei neri" << endl;
        }
        do {
            cout << "\n\nINSERT MOVE : ";
            getline(cin, input); 
            regex_search(input, coordinates, input_filter);
        }while(!regex_match(input, input_filter));
        short *mosse = conversion(coordinates);
                start.first = *(mosse + 0);
                start.second = *(mosse + 1);
                end.first = *(mosse + 2);
                end.second = *(mosse + 3);
        moveOutput = b.move(start, end, black);
        if(moveOutput == mossaEseguita){
            system("cls");
            cout << "ez funzia";
            black = !black;
        }
        else if(moveOutput == partitaInCulo){
            system("cls");
            cout << "cazzo culo";
            break;
        }
        else if(moveOutput == mossaNonValida){
            system("cls");
            cout << "hai cappellato coglione";
        }
    }
    return 0;
}