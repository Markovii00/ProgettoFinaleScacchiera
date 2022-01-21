#include <iostream>
#include <cctype>
#include <chrono>
#include <thread>
#include <string>
#include <algorithm>
#include <regex>
#include <random>
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


    vector<short> promotion_rand{'d', 'a', 't', 'c'};
    bool game = false;
    while (!game) {
        b.printBoard();

        vector<pair<coords, coords>> moves = b.getSetPossibleMoves(white);

        std::random_device rd;
        std::mt19937 engine(rd());
        std::uniform_int_distribution<int> dist(0, moves.size()-1);
        pair<coords, coords> move = moves.at(dist(engine));
        cout << "\n[" << move.first.first << move.first.second << "] -> [" << move.second.first << move.second.second << "]\n";
        pair<bool, int> res = b.move(move.first, move.second, white, false);

        cout <<"\n Pezzi neri: ["<<b.getBlackSet().size()<< "]\n";
        cout <<"\n Pezzi bianchi: ["<<b.getWhiteSet().size()<< "]\n";

        if (res.first) {
            cout << res.first;
            cout << res.second;
            switch (res.second) {
                case 1: {
                    cout << "\n1\n";
                    white = !white;
                    break;
                }

                case 2: {
                    cout << "\n2\n";
                    std::random_device rd;
                    std::mt19937 engine(rd());
                    std::uniform_int_distribution<int> dist(0, promotion_rand.size()-1);

                    b.promotion(promotion_rand.at(dist(engine)), white);
                    white = !white;
                    break;
                }

                case 3 : {
                    cout << "\n3\n";
                    cout << "Scacco matto, fine della partita!\n";
                    game = true;
                    b.printBoard();
                    return 0;
                }
            }
        } else {

            cout << res.first;
            cout << res.second;
            switch (res.second) {
                case 1: {
                    cout << "An invalid move?\n";
                    game = true;
                    break;
                }

                case 2 : {
                    cout << "Re sotto scacco, rieseguire la mossa\n";
                    break;
                }

                case 3 : {
                    cout << "Patta richiesta, ignoro la patta\n";
                    return 0;
                }

                case 4 : {
                    cout << "Forzo fine partita per patta\n";
                    return 0;
                }
            }
        }
    }

    return 0;
}